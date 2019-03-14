#include <signal.h>
#include "notify.hpp"
#include <boost/log/trivial.hpp>
#include "libpc2/pc2/pc2.hpp"
#include "libpc2/pc2/beo4.hpp"

PC2Notifier notifier;

class BeoportPC2Interface: public PC2Interface {
    public:
        BeoportPC2Interface() {
            this->address_mask = PC2Interface::address_mask_t::beoport;
        }

        void beo4_press(Beo4::keycode keycode) {
            BOOST_LOG_TRIVIAL(debug) << "Got Beo4 keycode " << (unsigned int) keycode;
            if (keycode == Beo4::keycode::tv) {
                this->pc2->mixer->transmit_locally(true);
                this->pc2->mixer->transmit_from_ml(true);
                this->pc2->mixer->speaker_power(true);
            }
            else if (keycode == Beo4::keycode::pc) {
                this->pc2->mixer->transmit_locally(true);
                this->pc2->mixer->transmit_from_ml(false);
                this->pc2->mixer->speaker_power(true);
            }
            else if (keycode == Beo4::keycode::vol_up) {
                this->pc2->mixer->adjust_volume(1);
                notifier.notify_volume(this->pc2->mixer->state.volume);
            }
            else if (keycode == Beo4::keycode::vol_down) {
                this->pc2->mixer->adjust_volume(-1);
                notifier.notify_volume(this->pc2->mixer->state.volume);
            }
            else if (keycode == Beo4::keycode::standby) {
                this->pc2->mixer->transmit_locally(false);
                this->pc2->mixer->transmit_from_ml(false);
                this->pc2->mixer->speaker_power(false);
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
        iface.pc2->keystroke_callback = std::bind(&BeoportPC2Interface::beo4_press, iface, std::placeholders::_1);
        pc.open();

        if(argv[1][0] == '1') {
            pc.mixer->transmit_locally(true);
            pc.mixer->speaker_power(true);
            pc.mixer->set_parameters(40, 0, 0, 0, false);
        } else if (argv[1][0] == '0') {
            pc.mixer->speaker_power(false);
        } else if (argv[1][0] == 't') {
            //pc.init();
            //MasterPresentTelegram query;
            //query.dest_node = 0xc0;
            //query.src_node = 0x01;
            //query.telegram_type = MasterlinkTelegram::telegram_types::request;
            //query.payload_version = 4;
            //query.payload = {0x0a, 0x01, 0x00};
            //pc.send_telegram(query);
            pc.device->send_message({ 0xE0, 0xC0, 0x01, 0x01, 0x0B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x01, 0x96, 0x00 });
            pc.event_loop(keepRunning);
        } else if (argv[1][0] == 'm') {
            //pc.notify = new PC2Notifier();
//            signal(SIGINT, intHandler);
//            signal(SIGTERM, intHandler);
            //pc.init();
            pc.event_loop(keepRunning);
            pc.mixer->transmit_locally(false);
            pc.mixer->speaker_power(false);
        }
    } else {
        printf("Usage: %s (figure out the rest from source)\n", argv[0]);
    }
}
