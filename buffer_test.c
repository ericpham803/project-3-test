#include "buffer.h"

int main(int argc, char *argv[]) {
    init_buffer_421();
    for(int i = 0; i < 20; i++) {
        call_prod_cons();
    }
    delete_buffer_421();
    return 0;
}