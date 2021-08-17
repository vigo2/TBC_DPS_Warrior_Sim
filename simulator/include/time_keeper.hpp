#ifndef WOW_SIMULATOR_TIME_KEEPER_HPP
#define WOW_SIMULATOR_TIME_KEEPER_HPP
#include <limits>

class Time_keeper
{
public:
    Time_keeper() = default;

    void increment(double next_event)
    {
        time = next_event + 1e-5;
    }

    void reset()
    {
        mortal_strike_cd_ = -1;
        sweeping_strikes_cd_ = -1;
        blood_thirst_cd_ = -1;
        rampage_cd_ = -1;
        overpower_cd_ = -1;
        whirlwind_cd_ = -1;
        global_cd_ = -1;

        time = 0.0;

        overpower_aura_ = -1;
        rampage_aura_ = -1;
    }

    void prepare(double prepare_time)
    {
        global_cd_ = prepare_time;
        time = prepare_time + 1e-5;
    }

    [[nodiscard]] double get_next_event(double next_mh_swing, double next_oh_swing,                                              
                                        double next_buff_event, double next_slam_finish, double sim_time) const
    {
        double next_event = std::numeric_limits<double>::max();
        if (overpower_cd_ >= time && overpower_cd_ < next_event) next_event = overpower_cd_;
        if (sweeping_strikes_cd_ >= time && sweeping_strikes_cd_ < next_event) next_event = sweeping_strikes_cd_;
        if (mortal_strike_cd_ >= time && mortal_strike_cd_ < next_event) next_event = mortal_strike_cd_;
        if (blood_thirst_cd_ >= time && blood_thirst_cd_ < next_event) next_event = blood_thirst_cd_;
        if (rampage_cd_ >= time && rampage_cd_ < next_event) next_event = rampage_cd_;
        if (whirlwind_cd_ >= time && whirlwind_cd_ < next_event) next_event = whirlwind_cd_;
        if (global_cd_ >= time && global_cd_ < next_event) next_event = global_cd_;
        if (next_mh_swing >= time && next_mh_swing < next_event) next_event = next_mh_swing;
        if (next_oh_swing >= time && next_oh_swing < next_event) next_event = next_oh_swing;
        if (next_slam_finish >= time && next_slam_finish < next_event) next_event = next_slam_finish;
        if (next_buff_event < next_event) next_event = next_buff_event;
        if (sim_time < next_event) next_event = sim_time;
        return next_event;
    }

    void overpower_cast(double cd) { overpower_cd_ = time + cd; }
    [[nodiscard]] bool overpower_ready() const { return overpower_cd_ < time; }
    [[nodiscard]] double overpower_cd() const { return overpower_cd_ - time; }

    void rampage_cast(double cd) { rampage_cd_ = time + cd; }
    [[nodiscard]] bool rampage_ready() const { return rampage_cd_ < time; }
    [[nodiscard]] double rampage_cd() const { return rampage_cd_ - time; }

    void sweeping_strikes_cast(double cd) { sweeping_strikes_cd_ = time + cd; }
    [[nodiscard]] bool sweeping_strikes_ready() const { return sweeping_strikes_cd_ < time; }
    [[nodiscard]] double sweeping_strikes_cd() const { return sweeping_strikes_cd_ - time; }

    void blood_thirst_cast(double cd) { blood_thirst_cd_ = time + cd; }
    [[nodiscard]] bool blood_thirst_ready() const { return blood_thirst_cd_ < time; }
    [[nodiscard]] double blood_thirst_cd() const { return blood_thirst_cd_ - time; }

    void mortal_strike_cast(double cd) { mortal_strike_cd_ = time + cd; }
    [[nodiscard]] bool mortal_strike_ready() const { return mortal_strike_cd_ < time; }
    [[nodiscard]] double mortal_strike_cd() const { return mortal_strike_cd_ - time; }

    void whirlwind_cast(double cd) { whirlwind_cd_ = time + cd; }
    [[nodiscard]] bool whirlwind_ready() const { return whirlwind_cd_ < time; }
    [[nodiscard]] double whirlwind_cd() const { return whirlwind_cd_ - time; }

    void global_cast(double cd) { global_cd_ = time + cd; }
    [[nodiscard]] bool global_ready() const { return global_cd_ < time; }
    [[nodiscard]] double global_cd() const { return global_cd_ - time; }

    void gain_overpower_aura() { overpower_aura_ = time + 5; }
    [[nodiscard]] bool can_do_overpower() const { return time <= overpower_aura_; }

    void gain_rampage_aura() { rampage_aura_ = time + 5; }
    [[nodiscard]] bool can_do_rampage() const { return time <= rampage_aura_; }

    double time;

private:
    double overpower_cd_;
    double rampage_cd_;
    double sweeping_strikes_cd_;
    double blood_thirst_cd_;
    double mortal_strike_cd_;
    double whirlwind_cd_;
    double global_cd_;

    double overpower_aura_;
    double rampage_aura_;
};

#endif // WOW_SIMULATOR_TIME_KEEPER_HPP
