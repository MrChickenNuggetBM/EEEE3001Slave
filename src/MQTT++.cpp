#include "MQTT++.h"
using namespace mqtt;
using namespace std;

const string SERVER_ADDRESS("mqtt://localhost:1883");
const string CLIENT_ID("raspberrypi");
const string TOPIC("parameters/minorRadius");

const int	QoS = 1;
const int	N_RETRY_ATTEMPTS = 5;

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual iaction_listener
{
    string name_;

    void on_failure(const token& tok) override {
        cout << name_ << " failure";
        if (tok.get_message_id() != 0)
            cout << " for token: [" << tok.get_message_id() << "]" << endl;
        cout << endl;
    }

    void on_success(const token& tok) override {
        cout << name_ << " success";
        if (tok.get_message_id() != 0)
            cout << " for token: [" << tok.get_message_id() << "]" << endl;
        auto top = tok.get_topics();
        if (top && !top->empty())
            cout << "\ttoken topic: '" << (*top)[0] << "', ..." << endl;
        cout << endl;
    }

public:
    action_listener(const string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual callback,
                 public virtual iaction_listener
{
    // Counter for the number of connection retries
    int nretry_;
    // The MQTT client
    const async_client& CLIENT;
    // Options to use if we need to reconnect
    const connect_options& CONN_OPTS;
    // An action listener to display the result of actions.
    const action_listener SUB_LISTENER;
    // array of topics to connect to
    const string* TOPICS;

    // This deomonstrates manually reconnecting to the broker by calling
    // connect() again. This is a possibility for an application that keeps
    // a copy of it's original connect_options, or if the app wants to
    // reconnect with different options.
    // Another way this can be done manually, if using the same options, is
    // to just call the async_client::reconnect() method.
    void reconnect() {
        this_thread::sleep_for(chrono::milliseconds(2500));
        try {
            CLIENT.connect(CONN_OPTS, nullptr, *this);
        }
        catch (const exception& exc) {
            cerr << "Error: " << exc.what() << endl;
            exit(1);
        }
    }

    // Re-connection failure
    void on_failure(const token& tok) override {
        cout << "Connection attempt failed" << endl;
        if (++nretry_ > N_RETRY_ATTEMPTS)
            exit(1);
        reconnect();
    }

    // (Re)connection success
    // Either this or connected() can be used for callbacks.
    void on_success(const token& tok) override {}

    // (Re)connection success
    void connected(const string& cause) override {
        cout << "\nConnection success" << endl;

        int i = 0;
        for (string topic = TOPICS[i]; i < TOPICS->length(); topic = TOPIC[++i]) {
            cout << "\nSubscribing to topic '" << TOPIC << "'\n"
                 << "\tfor client " << CLIENT_ID
                 << " using QoS" << QoS << "\n"
                 << "\nPress Q<Enter> to quit\n" << endl;

            CLIENT.subscribe(topic, QoS, nullptr, SUB_LISTENER);
        }
    }

    // Callback for when the connection is lost.
    // This will initiate the attempt to manually reconnect.
    void connection_lost(const string& cause) override {
        cout << "\nConnection lost" << endl;
        if (!cause.empty())
            cout << "\tcause: " << cause << endl;

        cout << "Reconnecting..." << endl;
        nretry_ = 0;
        reconnect();
    }

    // Callback for when a message arrives.
    void message_arrived(const_message_ptr msg) override {
        cout << "Message arrived" << endl;
        cout << "\ttopic: '" << msg->get_topic() << "'" << endl;
        cout << "\tpayload: '" << msg->to_string() << "'\n" << endl;
    }

    void delivery_complete(delivery_token_ptr token) override {}

public:
    callback(const async_client& cli, const connect_options& connOpts, const string* topics)
            : nretry_(0),
            CLIENT(cli),
            CONN_OPTS(connOpts),
            SUB_LISTENER("Subscription"),
            TOPICS(topics)
            {}
};

/////////////////////////////////////////////////////////////////////////////
