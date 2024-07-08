#include "performer.h"

struct Performer* find_performer(unsigned int performer_type) {
    struct Performer* performer_list = (struct Performer*) GROUND_STATE_PTRS.performers;

    //TODO: fill with the correct amount of max performer. Assuming 6 for now
    for (int i = 0; i < 6; i++) {
        if (performer_list[i].some_id == performer_type) {
            return &performer_list[i];
        }
    }

    return (struct Performer*) 0;
}