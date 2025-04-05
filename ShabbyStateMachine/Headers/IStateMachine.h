#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "ISubStateMachine.h"

class IStateMachine : public ISubStateMachine
{
public:
	virtual ~IStateMachine() = default;

	// IState interface
	void ContactedBy(std::shared_ptr<IState> contact_state) override = 0;
	void EnteredBy(std::shared_ptr<IState> enter_state) override = 0;
	void Execute() override = 0;
	void Suspend() override = 0;
	void RestoredFrom(std::shared_ptr<IState> src_state) override = 0;
	bool Transfer(std::shared_ptr<IState> dest_state) override = 0;
	std::string GetStatus() const override = 0;

	// IStateLinkageBuilder interface
	void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) override = 0;
	void SetInitialState(std::shared_ptr<IState> state) override = 0;
	void SetFinalState(std::shared_ptr<IState> state) override = 0;
	bool Validate() const override = 0;
};

#endif // STATEMACHINE_H