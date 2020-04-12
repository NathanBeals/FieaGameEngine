#pragma once

#pragma region Includes
// First Party
#include "Reaction.h"
#include "Stack.h"
#pragma endregion Includes

namespace Library
{
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

#pragma region Type Definitions
	public:
		/// <summary>
		/// Type used to distinguish EventMessageAttributed instances.
		/// </summary>
		using Subtype = std::string;
#pragma endregion Type Definitions

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		explicit ReactionAttributed(const std::string& name=std::string(), const Subtype& subtype=Subtype());

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~ReactionAttributed() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be copied.</param>
		ReactionAttributed(const ReactionAttributed& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be copied.</param>
		/// <returns>Newly copied into left hand side ReactionAttributed.</returns>
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be moved.</param>
		ReactionAttributed(ReactionAttributed&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be moved.</param>
		/// <returns>Newly moved into left hand side ReactionAttributed.</returns>
		ReactionAttributed& operator=(ReactionAttributed&& rhs) noexcept = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct Attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		explicit ReactionAttributed(const RTTI::IdType typeId, const std::string& name=std::string(), const Subtype& subtype=Subtype());
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ReactionAttributed.</returns>
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Event Subscriber Overrides
	public:
		/// <summary>
		/// Interface method called by an EventPublisher during Publish to receive the Event.
		/// </summary>
		/// <param name="eventPublisher">Reference to an Event as an EventPublisher.</param>
		virtual void Notify(EventPublisher& eventPublisher);
#pragma endregion Event Subscriber Overrides

#pragma region Scope Overrides
	public:
		/// <summary>
		/// Search override that first searches the top of the parameter stack, 
		/// then performs the base class Search implementation.
		/// </summary>
		/// <param name="key">KeyType value of the Attribute to be found.</param>
		/// <param name="scopePtrOut">Output parameter that points to the Scope which owns the found Attribute.</param>
		/// <returns>If found, a pointer to the DataType value of the Attribute. Otherwise, nullptr.</returns>
		virtual DataType* Search(const KeyType& key, Scope** scopePtrOut=nullptr) override;
#pragma endregion Scope Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Stack of Scopes maintaining the current set of auxiliary Attribute values from the 
		/// EventMessageAttributed instance passed by an Event at each call to Notify.
		/// These attributes act as a parameter list that is evaluated first during
		/// any Search call made during the ActionList Update call.
		/// </summary>
		Stack<Scope> mParameterStack;

		/// <summary>
		/// String to specify a EventMessageAttributed subtype to which this Reaction should respond.
		/// </summary>
		std::string mSubtype;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ReactionAttributedFactory class declaration.
	/// </summary>
	ConcreteFactory(ReactionAttributed, Scope)
#pragma endregion Factory
}