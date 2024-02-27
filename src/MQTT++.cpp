// Modified from async_subscribe.cpp && async_publish.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT subscriber using the C++ asynchronous client
// interface, employing callbacks to receive messages and status updates.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker.
//  - Subscribing to a topic
//  - Receiving messages through the callback API
//  - Receiving network disconnect updates and attempting manual reconnects.
//  - Using a "clean session" and manually re-subscribing to topics on
//    reconnect.
//

/*******************************************************************************
 * Copyright (c) 2013-2023 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#include "../include/MQTT++.h"

using namespace std;

namespace mqtt
{
// define variables
const int
QoS = 1,
N_RETRY_ATTEMPTS = 5;

namespace topics
{
namespace cv
{
char threshold = 175;
}
}


/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

void action_listener::on_failure(const token &tok)
{
    std::cout << name_ << " failure";
    if (tok.get_message_id() != 0)
        std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
    std::cout << std::endl;
}

void action_listener::on_success(const token &tok)
{
    std::cout << name_ << " success";
    if (tok.get_message_id() != 0)
        std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
    auto top = tok.get_topics();
    if (top && !top->empty())
        std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
    std::cout << std::endl;
}
action_listener::action_listener(const std::string &name) : name_(name) {}

void delivery_action_listener::on_failure(const token &tok)
{
    action_listener::on_failure(tok);
    done_ = true;
}

void delivery_action_listener::on_success(const token &tok)
{
    action_listener::on_success(tok);
    done_ = true;
}

delivery_action_listener::delivery_action_listener(const std::string &name) :
    action_listener(name),
    done_(false) {}

bool delivery_action_listener::is_done() const
{
    return done_;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
void Callback::reconnect()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try
    {
        CLIENT.connect(CONN_OPTS, nullptr, *this);
    }
    catch (const exception &exc)
    {
        std::cerr << "Error: " << exc.what() << std::endl;
        exit(1);
    }
}

// Re-connection failure
void Callback::on_failure(const token &tok)
{
    std::cout << "Connection attempt failed" << std::endl;
    if (++nretry_ > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}

// (Re)connection success
// Either this or connected() can be used for callbacks.
void Callback::on_success(const token &tok) {}

// (Re)connection success
void Callback::connected(const std::string &cause)
{
    std::cout << "\nConnection success" << std::endl
              << std::endl;

    int i = 0;
    for (std::string topic = TOPICS[i]; i < numTopics; topic = TOPICS[++i])
    {
        std::cout << "Subscribing to topic '" << topic << "'" << std::endl
                  << "\tfor client " << CLIENT.get_client_id() << " using QoS" << QoS << std::endl
                  << std::endl << std::flush;

        CLIENT.subscribe(topic, QoS, nullptr, SUB_LISTENER);
    }
}

// Callback for when the connection is lost.
// This will initiate the attempt to manually reconnect.
void Callback::connection_lost(const std::string &cause)
{
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
        std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    nretry_ = 0;
    reconnect();
}

// Callback for when a message arrives.
void Callback::message_arrived(const_message_ptr msg)
{
    std::string topic = msg->get_topic();
    std::string payload = msg->to_string();

    //std::cout << "Message arrived!" << std::endl;
    //std::cout << "\ttopic: '" << topic << "'" << std::endl;
    //std::cout << "\tpayload: '" << payload << "'\n" << std::endl;

    std::cout << topic << ": " << payload << std::endl;

    if (topic == "cv/threshold")
        topics::cv::threshold = std::stoi(payload);
}

void Callback::delivery_complete(delivery_token_ptr token)
{
    if(!token)
    {
        std::cout << "Delivery complete for token: -1"
                  << std::endl << std::flush;
        return;
    }

    std::cout << "Delivery complete for token: " << token->get_message_id()
              << std::endl << std::flush;

    return;
}

Callback::Callback(async_client &CLIENT, connect_options &connOpts, const std::string *topics, const int numtopics)
    : nretry_(0),
      CLIENT(CLIENT),
      CONN_OPTS(connOpts),
      SUB_LISTENER("Subscription"),
      TOPICS(topics),
      numTopics(numtopics) {}

// function to publish messages
std::shared_ptr<delivery_token> publishMessage(std::string topic, std::string payload, async_client& client)
{
    const char *_topic = topic.data();
    const char *_payload = payload.data();

    auto token = client.publish(_topic, _payload, strlen(_payload), QoS, false);

    // std::cout << std::endl << "Delivering: " << _topic << " = " << _payload << " [" << token->get_message_id() << "]"
    // << std::endl << std::flush;
    // << std::endl << std::flush;

    return token;
}

// function to publish images
std::shared_ptr<delivery_token> publishImage(std::string topic, cv::Mat frame, async_client& client)
{
    const char *_topic = topic.data();

    std::vector<uchar> frameBytes;
    cv::imencode(".jpg", frame, frameBytes);
    auto msg = make_message(_topic, frameBytes.data(), frameBytes.size());
    auto token = client.publish(msg);

    // std::cout << std::endl << "Delivering: " << _topic << " [" << token->get_message_id() << "]"
    // << std::endl << std::flush;

    return token;
}
}
