#include "MQTT++.h"

int main(int argc, char* argv[])
{
    // A subscriber often wants the server to remember its messages when its
    // disconnected. In that case, it needs a unique ClientID and a
    // non-clean session.

    async_client cli(SERVER_ADDRESS, CLIENT_ID);

    connect_options connOpts;
    connOpts.set_clean_session(false);

    // Install the callback(s) before connecting.
    callback cb(cli, connOpts);
    cli.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        cout << "Connecting to the MQTT server..." << flush;
        cli.connect(connOpts, nullptr, cb);
    }
    catch (const exception& exc) {
        cerr << "\nERROR: Unable to connect to MQTT server: '"
             << SERVER_ADDRESS << "'" << exc << endl;
        return 1;
    }

    // Just block till user tells us to quit.

    while (tolower(cin.get()) != 'q')
        ;

    // Disconnect

    try {
        cout << "\nDisconnecting from the MQTT server..." << flush;
        cli.disconnect()->wait();
        cout << "OK" << endl;
    }
    catch (const exception& exc) {
        cerr << exc << endl;
        return 1;
    }

    return 0;
}