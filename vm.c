#include "util.h"
#include <pulse/context.h>
#include <pulse/def.h>
#include <pulse/introspect.h>
#include <pulse/mainloop.h>
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <pulse/volume.h>
#include <stdio.h>

static pa_volume_t volume;

static void
get_sink_volume_callback(pa_context *c __attribute__((unused)),
                         const pa_sink_info *i, int is_last,
                         void *userdata __attribute__((unused)))
{
        if (is_last) return;

        if (i->mute) {
                volume = 0;
        } else {
                volume = (double) pa_cvolume_avg(&i->volume) / PA_VOLUME_NORM *
                         100.0;
        }

        pa_context_disconnect(c);
}

static void
context_state_callback(pa_context *c, void *userdata)
{
        switch (pa_context_get_state(c)) {
        case PA_CONTEXT_READY:
                pa_context_get_sink_info_by_index(
                    c, 0, get_sink_volume_callback, NULL);
                break;
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        case PA_CONTEXT_FAILED:
                // initial conn fails a few times before succeeding
                break;
        case PA_CONTEXT_TERMINATED:
                pa_mainloop_quit(userdata, 1);
                break;
        }
}

void
vm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%d%%";
        pa_mainloop *m;
        pa_mainloop_api *mapi;
        pa_context *c;

        m = pa_mainloop_new();
        mapi = pa_mainloop_get_api(m);
        c = pa_context_new(mapi, "statusbar");

        pa_context_connect(c, NULL, PA_CONTEXT_NOFLAGS, NULL);
        pa_context_set_state_callback(c, context_state_callback, m);
        pa_mainloop_run(m, NULL);

        pa_context_disconnect(c);
        pa_context_unref(c);
        pa_mainloop_free(m);
        snprintf(buff, bufflen, fmt, getIcon(arg, volume), volume);
}
