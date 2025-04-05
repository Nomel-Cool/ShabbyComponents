#pragma once
#ifndef SUBSTATEMACHINE_H
#define SUBSTATEMACHINE_H

#include "IState.h"
#include "IStateLinkageBuilder.h"

class ISubStateMachine : public IState, public IStateLinkageBuilder
{
public:
	virtual ~ISubStateMachine() = default;

	// IState interface
	virtual void ContactedBy(std::shared_ptr<IState> contact_state) override = 0;
	virtual void EnteredBy(std::shared_ptr<IState> enter_state) override = 0;
	virtual void Execute() override = 0;
	virtual void Suspend() override = 0;
	virtual void RestoredFrom(std::shared_ptr<IState> src_state) override = 0;
	virtual bool Transfer(std::shared_ptr<IState> src_state) override = 0;
	virtual std::string GetStatus() const override = 0;

	// IStateLinkageBuilder interface
	virtual void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) override = 0;
	virtual void SetInitialState(std::shared_ptr<IState> state) override = 0;
	virtual void SetFinalState(std::shared_ptr<IState> state) override = 0;
	virtual bool Validate() const override = 0;
};

#endif // SUBSTATEMACHINE_H