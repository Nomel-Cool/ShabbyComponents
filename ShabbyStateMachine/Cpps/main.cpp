#include <iostream>

#include <map>
#include <vector>

#include "IStateMachine.h"

class Sleep : public ISubStateMachine
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
		std::cout << enter_state->GetStatus() << " ---> " << m_current_state->GetStatus() << std::endl;
		Execute();
		Transfer(m_current_state);
	}
	void Execute() override
	{
		m_current_state->Execute();
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		for (const auto& linkage : m_linkages[src_state])
		{
			if (linkage.second())
			{
				m_current_state = linkage.first;
				m_current_state->EnteredBy(src_state);
				return true;
			}
		}
		return false;
	}
	void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) override
	{
		m_linkages[from].push_back(std::make_pair(to, condition));
	}
	void SetInitialState(std::shared_ptr<IState> state) override
	{
		m_current_state = state;
	}
	void SetFinalState(std::shared_ptr<IState> state) override
	{
		std::cout << "Setting final state" << std::endl;
	}
	bool Validate() const override
	{
		std::cout << "Validating state machine" << std::endl;
		return true;
	}
	std::string GetStatus() const override
	{
		return "Sleep";
	}
private:
	std::shared_ptr<IState> m_current_state;
	std::map<std::shared_ptr<IState>, std::vector<std::pair<std::shared_ptr<IState>, TransitionCondition>>> m_linkages;
};

class EyesClose : public IState
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
	}
	void Execute() override
	{
		std::cout << "Eyes closed." << std::endl;
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Eyes closed would not transfer." << std::endl;
		return false;
	}
	std::string GetStatus() const override
	{
		return "Eyes closed";
	}
};

class AWake : public ISubStateMachine
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
		std::cout << enter_state->GetStatus() << " ---> " << m_current_state->GetStatus() << std::endl;
	}
	void Execute() override
	{
		bool t = true;
		while (t)
		{
			m_current_state->Execute();
			t = Transfer(m_current_state);
		}
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		for (const auto& linkage : m_linkages[src_state])
		{
			if (linkage.second())
			{
				m_current_state = linkage.first;
				m_current_state->EnteredBy(src_state);
				return true;
			}
		}
		return false;
	}
	void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) override
	{
		m_linkages[from].push_back(std::make_pair(to, condition));
	}
	void SetInitialState(std::shared_ptr<IState> state) override
	{
		m_current_state = state;
	}
	void SetFinalState(std::shared_ptr<IState> state) override
	{
		std::cout << "Setting final state" << std::endl;
	}
	bool Validate() const override
	{
		std::cout << "Validating state machine" << std::endl;
		return true;
	}
	std::string GetStatus() const
	{
		return "Awake";
	}
private:
	std::shared_ptr<IState> m_current_state;
	std::map<std::shared_ptr<IState>, std::vector<std::pair<std::shared_ptr<IState>, TransitionCondition>>> m_linkages;
};

class EyesOn : public IState
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
	}
	void Execute() override
	{
		std::cout << "Eyes opened." << std::endl;
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Eyes open would not transfer." << std::endl;
		return false;
	}
	std::string GetStatus() const override
	{
		return "Eyes Open";
	}
};

class Play : public ISubStateMachine
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
		std::cout << enter_state->GetStatus() << " ---> " << m_current_state->GetStatus() << std::endl;
	}
	void Execute() override
	{
		bool t = true;
		while (t)
		{
			m_current_state->Execute();
			t = Transfer(m_current_state);
		}
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		for (const auto& linkage : m_linkages[src_state])
		{
			if (linkage.second())
			{
				m_current_state = linkage.first;
				m_current_state->EnteredBy(src_state);
				return true;
			}
		}
		return false;
	}
	void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) override
	{
		m_linkages[from].push_back(std::make_pair(to, condition));
	}
	void SetInitialState(std::shared_ptr<IState> state) override
	{
		m_current_state = state;
	}
	void SetFinalState(std::shared_ptr<IState> state) override
	{
		std::cout << "Setting final state" << std::endl;
	}
	bool Validate() const override
	{
		std::cout << "Validating state machine" << std::endl;
		return true;
	}
	std::string GetStatus() const override
	{
		return "Playing";
	}
private:
	std::shared_ptr<IState> m_current_state;
	std::map<std::shared_ptr<IState>, std::vector<std::pair<std::shared_ptr<IState>, TransitionCondition>>> m_linkages;
};

class Bark : public IState
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
		std::cout << enter_state->GetStatus() << " ---> " << GetStatus() << std::endl;
	}
	void Execute() override
	{
		std::cout << "Dog is barking." << std::endl;
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Bark would not transfer." << std::endl;
		return false;
	}
	std::string GetStatus() const override
	{
		return "Barking";
	}
};

class Growl : public IState
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
	}
	void Execute() override
	{
		std::cout << "Dog is growling." << std::endl;
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Growl would not transfer." << std::endl;
		return false;
	}
	std::string GetStatus() const override
	{
		return "Growling";
	}
};

class Dog : public IStateMachine
{
public:
	void ContactedBy(std::shared_ptr<IState> contact_state) override
	{
		std::cout << "Contacted by state" << std::endl;
	}
	void EnteredBy(std::shared_ptr<IState> enter_state) override
	{
		std::cout << m_current_state->GetStatus() << "--->" << enter_state->GetStatus() << std::endl;
	}
	void Execute() override
	{
		bool t = true;
		while (t)
		{
			m_current_state->Execute();
			t = Transfer(m_current_state);
		}
	}
	void Suspend() override
	{
		std::cout << "Suspending state" << std::endl;
	}
	void RestoredFrom(std::shared_ptr<IState> src_state) override
	{
		std::cout << "Restored from state" << std::endl;
	}
	bool Transfer(std::shared_ptr<IState> src_state) override
	{
		for (const auto& linkage : m_linkages[src_state])
		{
			if (linkage.second())
			{
				m_current_state = linkage.first;
				m_current_state->EnteredBy(src_state);
				return true;
			}
		}
		return false;
	}
	void AddTransition(std::shared_ptr<IState> from, std::shared_ptr<IState> to, TransitionCondition condition) override
	{
		m_linkages[from].push_back(std::make_pair(to, condition));
	}
	void SetInitialState(std::shared_ptr<IState> state) override
	{
		m_current_state = state;
	}
	void SetFinalState(std::shared_ptr<IState> state) override
	{
		std::cout << "Setting final state" << std::endl;
	}
	bool Validate() const override
	{
		std::cout << "Validating state machine" << std::endl;
		return true;
	}
	std::string GetStatus() const override
	{
		return "Dog is in the state machine.";
	}
private:
	std::shared_ptr<IState> m_current_state;
	std::map<std::shared_ptr<IState>, std::vector<std::pair<std::shared_ptr<IState>, TransitionCondition>>> m_linkages;
};

struct SomeConditions
{
	int i = 0;
};

int main()
{

	SomeConditions sc;

	std::shared_ptr<EyesOn> eyesOn = std::make_shared<EyesOn>();
	std::shared_ptr<EyesClose> eyesClose = std::make_shared<EyesClose>();
	std::shared_ptr<Bark> bark = std::make_shared<Bark>();
	std::shared_ptr<Growl> growl = std::make_shared<Growl>();

	std::shared_ptr<AWake> awake = std::make_shared<AWake>();
	awake->AddTransition(eyesClose, eyesOn, []() { return true; });

	std::shared_ptr<Play> play = std::make_shared<Play>();
	play->AddTransition(bark, growl, []() { return true; });

	std::shared_ptr<Sleep> sleep = std::make_shared<Sleep>();
	sleep->AddTransition(eyesOn, eyesClose, []() { return true; });

	Dog dog;
	dog.AddTransition(awake, play, [&sc]() { return sc.i == 0; });
	dog.AddTransition(play, sleep, []() { return true; });
	dog.AddTransition(sleep, awake, []() { return true; });

	dog.SetInitialState(awake);
	awake->SetInitialState(eyesClose);
	play->SetInitialState(bark);
	sleep->SetInitialState(eyesOn);
	dog.Execute();

	return 0;
}