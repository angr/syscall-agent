#pragma once

#include "panic.h"

#define trace_user_handle_signal(x, y) (0)
#define trace_signal_do_sigaction_host(x, y) (0)
#define trace_signal_do_sigaction_guest(x, y) (0)
#define trace_user_host_signal(x, y, z) (0)
#define trace_user_queue_signal(x, y) (0)
#define trace_user_force_sig(x, y, z) (0)
#define trace_signal_table_init(x) (0)
#define trace_event_get_state_backends(x) (0)
#define TRACE_SIGNAL_TABLE_INIT 0
#define cpu_signal_handler(x, y, z) (PANIC_UNREACHED(), 0)
#define gdb_signalled(x, y) PANIC_UNREACHED()
#define gdb_handlesig(x, y) (PANIC_UNREACHED(), 0)
