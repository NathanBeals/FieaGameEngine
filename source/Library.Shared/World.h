#pragma once

// First Party
#include "Attributed.h"
#include "WorldState.h"

namespace Library
{
	// Forwarded Classes
	class Sector;

	/// <summary>
	/// Represents the global scope for a simulation, containing sectors and their entities.
	/// </summary>
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Data structure for performing an action on a given Scope at the end of an Update call.
		/// </summary>
		struct PendingChild final
		{
			/// <summary>
			/// Defines the state delimiting which action is to be performed on the PendingChild.
			/// </summary>
			enum class State
			{
				Invalid = -1,

				ToAdd,
				ToRemove,

				End
			};

			/// <summary>
			/// Child Scope pending an action.
			/// </summary>
			Scope& Child;
			
			/// <summary>
			/// Child pending State delimiting which action is to be performed on the Child.
			/// </summary>
			const State ChildState;
			
			/// <summary>
			/// Target Scope of the performed action.
			/// </summary>
			Scope& Target;

			/// <summary>
			/// Name of the Target attribute of the performed action.
			/// </summary>
			const NameType* const AttributeName;
		};

		/// <summary>
		/// Type definition for a list of PendingChild data.
		/// </summary>
		using PendingChildList = Vector<PendingChild>;

	public:
		/// <summary>
		/// Key for the Name attribute in the World.
		/// </summary>
		inline static const std::string NameKey = "Name";

		/// <summary>
		/// Key for the Sectors attribute in the World.
		/// </summary>
		inline static const std::string SectorsKey = "Sectors";

		/// <summary>
		/// Index of the Sectors attribute in the World.
		/// </summary>
		inline static const std::size_t SectorsIndex = 2;

	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion Type Definitions, Constants

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Name of the World.</param>
		explicit World(const std::string& name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~World() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">World to be copied.</param>
		World(const World & rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">World to be copied.</param>
		/// <returns>Newly copied into left hand side World.</returns>
		World& operator=(const World & rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">World to be moved.</param>
		World(World && rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">World to be moved.</param>
		/// <returns>Newly moved into left hand side World.</returns>
		World& operator=(World && rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the World.</returns>
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Gets the list of PendingChild data.
		/// </summary>
		/// <returns>Current PendingChild data.</returns>
		PendingChildList& PendingChildren();

		/// <summary>
		/// Gets the list of PendingChild data.
		/// </summary>
		/// <returns>Current PendingChild data.</returns>
		const PendingChildList& PendingChildren() const;

		/// <summary>
		/// Name of the World.
		/// </summary>
		/// <returns>Name of the World as a std::string.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Sets the name of the World.
		/// </summary>
		/// <param name="name">String to use as the name of the World.</param>
		void SetName(const std::string & name);

		/// <summary>
		/// Gets the data handle to the Sector objects contained in this World.
		/// </summary>
		/// <returns>Reference to the Sector objects.</returns>
		DataType& Sectors();

		/// <summary>
		/// Gets the data handle to the Sector objects contained in this World.
		/// </summary>
		/// <returns>Reference to the Sector objects.</returns>
		const DataType& Sectors() const;

		/// <summary>
		/// Generates an Sector class and adopts it into this World.
		/// </summary>
		/// <param name="name">Name of the newly created Sector.</param>
		/// <returns>Reference to the newly heap allocated Sector.</returns>
		Sector& CreateSector(const std::string& name);
#pragma endregion Accessors

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method to be called every frame.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		void Update();
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the World as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Helper Methods
	private:
		/// <summary>
		/// Performs pending actions of the child Scopes.
		/// </summary>
		void UpdatePendingChildren();
#pragma endregion Helper Methods

#pragma region Data Members
	private:
		/// <summary>
		/// Clock that tracks the game start time, current time, and update delta time.
		/// </summary>
		GameClock mGameClock;

		/// <summary>
		/// Convenience struct for passing the WorldState data in cascaded Update calls.
		/// </summary>
		WorldState mWorldState;

 		/// <summary>
 		/// Pending children to have an action performed during the end of an Update call.
 		/// </summary>
 		PendingChildList mPendingChildren{ PendingChildList(PendingChildList::EqualityFunctor()) };

		/// <summary>
		/// Name of the World, reflected as a prescribed attribute.
		/// </summary>
		std::string mName;

		/// <summary>
		/// Collection of World objects within the Sectors prescribed attribute.
		/// </summary>
		DataType& mSectors;
#pragma endregion Data Members
	};
}
