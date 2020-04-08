#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionExpression.h"

// Standard
#include <sstream>

// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionExpression::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ ExpressionKey, Types::String, false, 1, offsetof(ActionExpression, mExpression) },
				{ ResultKey, Types::Reference, false, 1, offsetof(ActionExpression, mResultPtr) }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionExpression::ActionExpression(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	ActionExpression::ActionExpression(ActionExpression&& rhs) noexcept : Action(std::move(rhs)), 
		mResult(std::move(rhs.mResult))
	{
	}

	ActionExpression& ActionExpression::operator=(ActionExpression&& rhs) noexcept
	{
		Action::operator=(std::move(rhs));
		mResult = std::move(rhs.mResult);

		return *this;
	}

	gsl::owner<Library::Scope*> ActionExpression::Clone() const
	{
		return new ActionExpression(*this);
	}

	void ActionExpression::Update(WorldState&)
	{
		TokenizeExpression();
	}

	std::string ActionExpression::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << ": '" << mResult.ToString() << "' (ActionExpression)";
		return oss.str();
	}

	void ActionExpression::TokenizeExpression()
	{
		std::istringstream iss(mExpression);
	}
}