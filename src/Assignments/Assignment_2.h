#pragma once

#include "../Common/Assignment.h"

class Assignment_2 final : public Assignment
{
public:
	Assignment_2();
	~Assignment_2() override;

private:

	void EnterLabyrinth(int thread_num);

	int m_guestCount;

	std::mutex m_eatCupcake;

	std::unique_lock<std::mutex> m_cupCake;

	std::atomic_int m_leaderCount = 1;

	std::vector<bool> m_prisonerCupcakes;

};
