#ifndef WOW_SIMULATOR_TIME_KEEPER_HPP
#define WOW_SIMULATOR_TIME_KEEPER_HPP

class Time_keeper
{
public:
    Time_keeper() = default;

    void increment(double dt)
    {
        time += dt;
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

    [[nodiscard]] double get_dynamic_time_step(double mh_dt, double oh_dt, double buff_dt, double sim_dt, double slam_dt) const
    {
        double dt = std::numeric_limits<double>::max();
        if (overpower_cd_ >= time && overpower_cd_ < dt) dt = overpower_cd_;
        if (sweeping_strikes_cd_ >= time && sweeping_strikes_cd_ < dt) dt = sweeping_strikes_cd_;
        if (mortal_strike_cd_ >= time && mortal_strike_cd_ < dt) dt = mortal_strike_cd_;
        if (blood_thirst_cd_ >= time && blood_thirst_cd_ < dt) dt = blood_thirst_cd_;
        if (rampage_cd_ >= time && rampage_cd_ < dt) dt = rampage_cd_;
        if (whirlwind_cd_ >= time && whirlwind_cd_ < dt) dt = whirlwind_cd_;
        if (global_cd_ >= time && global_cd_ < dt) dt = global_cd_;
        dt -= time;
        if (slam_dt >= 0.0 && slam_dt < dt) dt = slam_dt;
        if (mh_dt < dt) dt = mh_dt;
        if (oh_dt < dt) dt = oh_dt;
        if (buff_dt < dt) dt = buff_dt;
        if (sim_dt < dt) dt = sim_dt;
        dt += 1e-5;
        return dt;
    }

    [[nodiscard]] double get_dynamic_time_step_old(double mh_dt, double oh_dt, double buff_dt, double sim_dt, double slam_dt) const
    {
        double dt = 100.0;
        if (overpower_cd_ >= time)
        {
            dt = std::min(overpower_cd_ - time, dt);
        }
        if (sweeping_strikes_cd_ >= time)
        {
            dt = std::min(sweeping_strikes_cd_ - time, dt);
        }
        if (mortal_strike_cd_ >= time)
        {
            dt = std::min(mortal_strike_cd_ - time, dt);
        }
        if (blood_thirst_cd_ >= time)
        {
            dt = std::min(blood_thirst_cd_ - time, dt);
        }
        if (rampage_cd_ >= time)
        {
            dt = std::min(rampage_cd_ - time, dt);
        }
        if (whirlwind_cd_ >= time)
        {
            dt = std::min(whirlwind_cd_ - time, dt);
        }
        if (global_cd_ >= time)
        {
            dt = std::min(global_cd_ - time, dt);
        }
        if (slam_dt >= 0.0)
        {
            dt = std::min(slam_dt, dt);
        }
        dt = std::min(mh_dt, dt);
        dt = std::min(oh_dt, dt);
        dt = std::min(buff_dt, dt);
        dt = std::min(sim_dt, dt);
        dt += 1e-5;
        return dt;
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
