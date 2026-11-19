#ifndef COMPOSITESTRATEGY_HPP
#define COMPOSITESTRATEGY_HPP
#include "Interfaces/Interfaces.hpp"
#include "Trading/Common/IStrategy.hpp"
#include <memory>
#include <vector>

namespace Core::Trading
{

class CompositeStrategy final : public IStrategy
{
public:

    explicit CompositeStrategy(std::unique_ptr<IEntryRule> entry,
                                std::unique_ptr<IExitRule> exit,
                                std::vector<std::unique_ptr<IFilter>> filters,
                                std::unique_ptr<ISizer> sizer);

    void setInst(const InstrumentState& inst);
    Signal onClosedKline(const Context& ctx) override;
    void reset() override;

private:

    InstrumentState m_inst;
    std::unique_ptr<IEntryRule> m_entry;
    std::unique_ptr<IExitRule> m_exit;
    std::vector<std::unique_ptr<IFilter>> m_filters;
    std::unique_ptr<ISizer> m_sizer;

};

}


#endif // COMPOSITESTRATEGY_HPP
