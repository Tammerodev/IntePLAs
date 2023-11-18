#pragma once

namespace Session {
    enum SessionState {
        None, Host, Join, SinglePlayer
    };

    inline SessionState session = None;
}