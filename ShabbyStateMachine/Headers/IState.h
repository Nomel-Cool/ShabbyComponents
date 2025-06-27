#pragma once
#ifndef ISTATE_H
#define ISTATE_H

#include <memory>
#include <string>

class IState
{
public:
	virtual ~IState() = default;
	virtual void ContactedBy(std::shared_ptr<IState> contact_state) = 0;
	virtual void EnteredBy(std::shared_ptr<IState> enter_state) = 0;
	virtual void Execute() = 0;
	virtual void Suspend() = 0;
	virtual void RestoredFrom(std::shared_ptr<IState> src_state) = 0;
	virtual bool Transfer(std::shared_ptr<IState> dest_state) = 0;
	virtual void TransferAsync(std::shared_ptr<IState> dest_state, TransitionCondition condition) = 0;
	virtual void PushHistory(std::shared_ptr<IState> state) = 0;
	virtual std::shared_ptr<IState> PopHistory() = 0;
	virtual std::string GetStatus() const = 0;
};

#endif // !ISTATE_H