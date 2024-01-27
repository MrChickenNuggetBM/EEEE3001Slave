// Modified from async_subscribe.cpp
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

#ifndef MQTTPP_H
#define MQTTPP_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"s

namespace mqtt {
    const int	QoS = 1;
    const int	N_RETRY_ATTEMPTS = 5;

    namespace topics {
        namespace parameters {
            static int xCenter = 0,
                    yCenter = 0,
                    xRadius = 960,
                    yRadius = 540,
                    thickness = 3;

            static bool isCircle = false,
                    isBrightfield = true,
                    isGUIControl = false;
        }
    }

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

    class action_listener : public virtual iaction_listener {
        std::string name_;

        void on_failure(const token &tok) override {
            std::cout << name_ << " failure";
            if (tok.get_message_id() != 0)
                std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
            std::cout << std::endl;
        }

        void on_success(const token &tok) override {
            std::cout << name_ << " success";
            if (tok.get_message_id() != 0)
                std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
            auto top = tok.get_topics();
            if (top && !top->empty())
                std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
            std::cout << std::endl;
        }

    public:
        action_listener(const std::string &name) : name_(name) {}
    };

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
    class Callback : public virtual callback,
                     public virtual iaction_listener {
        // Counter for the number of connection retries
        int nretry_;
        // The MQTT client
        async_client &CLIENT;
        // Options to use if we need to reconnect
        connect_options &CONN_OPTS;
        // An action listener to display the result of actions.
        action_listener SUB_LISTENER;
        // array of topics to connect to
        const char numTopics;
        const std::string *TOPICS;

        // This deomonstrates manually reconnecting to the broker by calling
        // connect() again. This is a possibility for an application that keeps
        // a copy of it's original connect_options, or if the app wants to
        // reconnect with different OPTIONS.
        // Another way this can be done manually, if using the same OPTIONS, is
        // to just call the async_client::reconnect() method.
        void reconnect() {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            try {
                CLIENT.connect(CONN_OPTS, nullptr, *this);
            }
            catch (const exception &exc) {
                std::cerr << "Error: " << exc.what() << std::endl;
                exit(1);
            }
        }

        // Re-connection failure
        void on_failure(const token &tok) override {
            std::cout << "Connection attempt failed" << std::endl;
            if (++nretry_ > N_RETRY_ATTEMPTS)
                exit(1);
            reconnect();
        }

        // (Re)connection success
        // Either this or connected() can be used for callbacks.
        void on_success(const token &tok) override {}

        // (Re)connection success
        void connected(const std::string &cause) override {
            std::cout << "\nConnection success" << std::endl
                      << std::endl;

            int i = 0;
            for (std::string topic = TOPICS[i]; i < numTopics; topic = TOPICS[++i]) {
                std::cout << "\nSubscribing to topic '" << topic << "'" << std::endl
                          << "\tfor client " << CLIENT.get_client_id() << " using QoS" << QoS << std::endl
                          << std::flush;

                CLIENT.subscribe(topic, QoS, nullptr, SUB_LISTENER);
            }
        }

        // Callback for when the connection is lost.
        // This will initiate the attempt to manually reconnect.
        void connection_lost(const std::string &cause) override {
            std::cout << "\nConnection lost" << std::endl;
            if (!cause.empty())
                std::cout << "\tcause: " << cause << std::endl;

            std::cout << "Reconnecting..." << std::endl;
            nretry_ = 0;
            reconnect();
        }

        // Callback for when a message arrives.
        void message_arrived(const_message_ptr msg) override {
            std::string topic = msg->get_topic();
            std::string payload = msg->to_string();

            std::cout << "Message arrived!" << std::endl;
            std::cout << "\ttopic: '" << topic << "'" << std::endl;
            std::cout << "\tpayload: '" << payload << "'\n" << std::endl;

            if (topic == "parameters/xCenter")
                topics::parameters::xCenter = std::stoi(payload);
            else if (topic == "parameters/yCenter")
                topics::parameters::yCenter = std::stoi(payload);
            else if (topic == "parameters/xRadius")
                topics::parameters::xRadius = std::stoi(payload);
            else if (topic == "parameters/yRadius")
                topics::parameters::yRadius = std::stoi(payload);
            else if (topic == "parameters/thickness")
                topics::parameters::thickness = std::stoi(payload);
            else if (topic == "parameters/isCircle")
                topics::parameters::isCircle = (payload == "true");
            else if (topic == "parameters/isBrightfield")
                topics::parameters::isBrightfield = (payload == "true");
            else if (topic == "parameters/isGUIControl")
                topics::parameters::isGUIControl = (payload == "true");
        }

        void delivery_complete(delivery_token_ptr token) override {}

    public:
        Callback(async_client &CLIENT, connect_options &connOpts, const std::string *topics,
                 const int numtopics)
                : nretry_(0),
                  CLIENT(CLIENT),
                  CONN_OPTS(connOpts),
                  SUB_LISTENER("Subscription"),
                  TOPICS(topics),
                  numTopics(numtopics) {}
    };

}

#endif // MQTTPP_H
