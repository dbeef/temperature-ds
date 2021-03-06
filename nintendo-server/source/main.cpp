/*---------------------------------------------------------------------------------

NDS streaming receiver, based on wifi demo by dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <dswifi9.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <nds/arm9/input.h>
#include <nds/arm9/console.h>
#include <nds/arm9/keyboard.h>
#include <nds/arm9/decompress.h>
#include <nds/arm7/touch.h>

#define PORT 8080

void set_up_server();

void receive_transmission() {


    u32 keys_held{};
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    printf("[*] Setting socket file descriptor\n");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("[-] Setting socket file descriptor failed\n");
        exit(EXIT_FAILURE);
    }

    printf("[*] Setting file descriptor\n");
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("[-] Setting file descriptor failed\n");
        exit(EXIT_FAILURE);
    }
    printf("[+] Done setting file descriptor\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("[*] Binding socket to port %i\n", PORT);
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("[-] Binding socket failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Done binding socket to port %i\n", PORT);

    printf("[*] Listening for connections\n");
    if (listen(server_fd, 3) < 0) {
        perror("[*] Error on listening for connection\n");
        exit(EXIT_FAILURE);
    }

    printf("[*] Accepting connection\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                             &addrlen)) < 0) {
        perror("[-] Failed accepting connection\n");
        exit(EXIT_FAILURE);
    }

    int stat;
    char keys[12];

    for (;;) {

        char ack{};

        //Retrieve ACK
        do {
            stat = recv(new_socket, &ack, sizeof(ack), 0);
            if (stat == 1) {
                break;
            }
        } while (stat < 0);

        scanKeys();
        keys_held = keysHeld();

        for (char &key : keys)
            key = '0';

        if ((keys_held & KEY_RIGHT) != 0)
            keys[0] = '1';
        if ((keys_held & KEY_LEFT) != 0)
            keys[1] = '1';
        if ((keys_held & KEY_DOWN) != 0)
            keys[2] = '1';
        if ((keys_held & KEY_UP) != 0)
            keys[3] = '1';
        if ((keys_held & KEY_X) != 0)
            keys[4] = '1';
        if ((keys_held & KEY_B) != 0)
            keys[5] = '1';
        if ((keys_held & KEY_Y) != 0)
            keys[6] = '1';
        if ((keys_held & KEY_A) != 0)
            keys[7] = '1';
        if ((keys_held & KEY_L) != 0)
            keys[8] = '1';
        if ((keys_held & KEY_R) != 0)
            keys[9] = '1';
        if ((keys_held & KEY_SELECT) != 0)
            keys[10] = '1';
        if ((keys_held & KEY_START) != 0)
            keys[11] = '1';

        do {
            stat = send(new_socket, keys, 12 * sizeof(char), 0);
        } while (stat < 0);

    }
}

//---------------------------------------------------------------------------------
Wifi_AccessPoint *findAP(void) {
//---------------------------------------------------------------------------------

    int selected = 0;
    int i;
    int count = 0, displaytop = 0;

    static Wifi_AccessPoint ap;

    Wifi_ScanMode(); //this allows us to search for APs

    int pressed = 0;
    do {

        scanKeys();
        pressed = keysDown();

        if (pressed & KEY_START) exit(0);

        //find out how many APs there are in the area
        count = Wifi_GetNumAP();


        consoleClear();

        printf("%d APs detected\n\n", count);

        int displayend = displaytop + 10;
        if (displayend > count) displayend = count;

        //display the APs to the user
        for (i = displaytop; i < displayend; i++) {
            Wifi_AccessPoint ap;

            Wifi_GetAPData(i, &ap);

            // display the name of the AP
            printf("%s %.29s\n  Wep:%s Sig:%i\n",
                   i == selected ? "*" : " ",
                   ap.ssid,
                   ap.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
                   ap.rssi * 100 / 0xD0);

        }

        //move the selection asterick
        if (pressed & KEY_UP) {
            selected--;
            if (selected < 0) {
                selected = 0;
            }
            if (selected < displaytop) displaytop = selected;
        }

        if (pressed & KEY_DOWN) {
            selected++;
            if (selected >= count) {
                selected = count - 1;
            }
            displaytop = selected - 9;
            if (displaytop < 0) displaytop = 0;
        }
        swiWaitForVBlank();
    } while (!(pressed & KEY_A));

    //user has made a choice so grab the ap and return it
    Wifi_GetAPData(selected, &ap);

    return &ap;
}

//---------------------------------------------------------------------------------
void keyPressed(int c) {
//---------------------------------------------------------------------------------
    if (c > 0) printf("%c", c);
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
    Wifi_InitDefault(false);

    consoleDemoInit();

    Keyboard *kb = keyboardDemoInit();
    kb->OnKeyPressed = keyPressed;

    while (1) {
        int status = ASSOCSTATUS_DISCONNECTED;

        consoleClear();
        consoleSetWindow(NULL, 0, 0, 32, 24);

        Wifi_AccessPoint *ap = findAP();

        consoleClear();
        consoleSetWindow(NULL, 0, 0, 32, 10);

        printf("Connecting to %s\n", ap->ssid);

        //this tells the wifi lib to use dhcp for everything
        Wifi_SetIP(0, 0, 0, 0, 0);
        char wepkey[64];
        int wepmode = WEPMODE_NONE;
        if (ap->flags & WFLAG_APDATA_WEP) {
            printf("Enter Wep Key\n");
            while (wepmode == WEPMODE_NONE) {
                scanf("%s", wepkey);
                if (strlen(wepkey) == 13) {
                    wepmode = WEPMODE_128BIT;
                } else if (strlen(wepkey) == 5) {
                    wepmode = WEPMODE_40BIT;
                } else {
                    printf("Invalid key!\n");
                }
            }
            Wifi_ConnectAP(ap, wepmode, 0, (u8 *) wepkey);
        } else {
            Wifi_ConnectAP(ap, WEPMODE_NONE, 0, 0);
        }
        consoleClear();
        while (status != ASSOCSTATUS_ASSOCIATED && status != ASSOCSTATUS_CANNOTCONNECT) {

            status = Wifi_AssocStatus();
            int len = strlen(ASSOCSTATUS_STRINGS[status]);
            printf("\x1b[0;0H\x1b[K");
            printf("\x1b[0;%dH%s", (32 - len) / 2, ASSOCSTATUS_STRINGS[status]);

            scanKeys();

            if (keysDown() & KEY_B) break;

            swiWaitForVBlank();
        }

        if (status == ASSOCSTATUS_ASSOCIATED) {
            u32 ip = Wifi_GetIP();

            printf("\nip: [%li.%li.%li.%li]\n", (ip) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);

            printf("[*] Setting up server:\n");
            set_up_server();
            receive_transmission();

        } else {
            printf("\nConnection failed!\n");
        }

        int quit = 0;
        printf("Press A to try again, B to quit.");
        while (1) {
            swiWaitForVBlank();
            scanKeys();
            int pressed = keysDown();
            if (pressed & KEY_B) quit = 1;
            if (pressed & (KEY_A | KEY_B)) break;
        }
        if (quit) break;
    }
    return 0;
}

void set_up_server() {
    consoleDemoInit();
}