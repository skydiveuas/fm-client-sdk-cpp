#ifndef PILOTSIMULATOR_HPP
#define PILOTSIMULATOR_HPP

#include "ISimulator.hpp"

#include "Pilot.hpp"

#include "event/input/user/UserEvent.hpp"

class PilotSimulator : public ISimulator
{
public:
    PilotSimulator(boost::asio::io_service&);

    void start(const std::string&, const int, const std::string&);

private:
    std::unique_ptr<fm::Pilot> pilot;

    void handleEvent(const std::shared_ptr<const fm::event::output::FacadeEvent> event) override;

    void emmitEvent(std::shared_ptr<const fm::event::input::user::UserEvent>, long);
};

#endif // PILOTSIMULATOR_HPP
