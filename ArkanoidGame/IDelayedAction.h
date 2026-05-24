#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace ArkanoidGame
{
	class IDelayedAction
	{
	protected:
		bool isTimerStarted_ = false;
		float destroyTime_ = 0;
		float currentTime_ = 0;

		virtual void UpdateTimer(float timeDelta)
		{
			if (!isTimerStarted_)
			{
				return;
			}
			currentTime_ -= timeDelta;
			EachTickAction(timeDelta);
			if (currentTime_ <= 0)
			{
				FinalAction();
				isTimerStarted_ = false;
			}
		}

		virtual void FinalAction() = 0;
		virtual void EachTickAction(float timeDelta) = 0;

	public:
		bool IsTimerStarted() const { return isTimerStarted_; }
		void StartTimer(float destroyTime)
		{
			destroyTime_ = destroyTime;
			currentTime_ = destroyTime;
			isTimerStarted_ = true;
		}
	};
}