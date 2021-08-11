#ifndef WOW_SIMULATOR_RAGE_MANAGER_HPP
#define WOW_SIMULATOR_RAGE_MANAGER_HPP

class Rage_manager
{
public:
    virtual void gain_rage(double amount) = 0;
    virtual void spend_rage(double amount) = 0;

    virtual void spend_all_rage() = 0;
    virtual void swap_stance() = 0;

    [[nodiscard]] virtual double get_rage() const = 0;
};

#endif // WOW_SIMULATOR_RAGE_MANAGER_HPP
