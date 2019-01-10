#include <signal.h>
#include "notify.hpp"
#include <boost/log/trivial.hpp>
#include "libpc2/pc2/pc2.hpp"
#include "libpc2/pc2/beo4.hpp"

PC2Notifier notifier;

class BeoportPC2Interface: public PC2Interface {
    void beo4_press(uint8_t keycode) {
        BOOST_LOG_TRIVIAL(debug) << "Got Beo4 keycode " << (unsigned int) keycode;
        if (keycode == BEO4_KEY_PC) {
            this->pc2->mixer->transmit_locally(true);
            this->pc2->mixer->transmit_from_ml(false);
            this->pc2->mixer->speaker_power(true);
        }
        // Volume up
        else if (keycode == 0x60) {
            this->pc2->mixer->adjust_volume(1);
            notifier.notify_volume(this->pc2->mixer->state.volume);
        }
        // Volume down
        else if (keycode == 0x64) {
            this->pc2->mixer->adjust_volume(-1);
            notifier.notify_volume(this->pc2->mixer->state.volume);
        }
    }
};

static volatile bool keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char *argv[]) {
    if(argc == 2) {
        BeoportPC2Interface iface;
        PC2 pc(&iface);
        pc.open();

        if(argv[1][0] == '1') {
            pc.mixer->transmit_locally(true);
            pc.mixer->speaker_power(true);
        } else if (argv[1][0] == '0') {
            pc.mixer->speaker_power(false);
        } else if (argv[1][0] == 'm') {
            //pc.notify = new PC2Notifier();
//            signal(SIGINT, intHandler);
//            signal(SIGTERM, intHandler);
            pc.init();
            pc.mixer->set_parameters(0x22, 0, 0, 0, false);
            pc.event_loop(keepRunning);
            pc.mixer->transmit_locally(false);
            pc.mixer->speaker_power(false);
        }
    } else {
        printf("Usage: %s (figure out the rest from source)\n", argv[0]);
    }
}
