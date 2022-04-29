#pragma once

static class Protocol
{
public:
	
	// Client Headers
	static struct C
	{
		static enum class WellcomeChallenge { QUESTION_ID, RESPONSE_ID };
		static enum class WelcomeSALT {};
	};

	// Server Headers
	static struct S
	{
		static enum class WellcomeChallenge { RESPONSE_ID, WELCOME_ID };
		static enum class WelcomeSALT {};
	};
};