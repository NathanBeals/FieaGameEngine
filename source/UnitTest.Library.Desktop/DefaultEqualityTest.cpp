#include "pch.h"

#include "DefaultEquality.h"
#include "Foo.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;
using namespace Library::Equality;


namespace UtilityTests
{
	TEST_CLASS(DefaultEqualityTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(IntEquality)
		{
			DefaultEquality<int> equality;

			int a = 10;
			int b = 20;
			int c(a);

			Assert::IsTrue(equality(a, a));
			Assert::IsFalse(equality(a, b));
			Assert::IsTrue(equality(a, c));
		}

		TEST_METHOD(DoubleEquality)
		{
			DefaultEquality<double> equality;

			double a = 10;
			double b = 20;
			double c(a);

			Assert::IsTrue(equality(a, a));
			Assert::IsFalse(equality(a, b));
			Assert::IsTrue(equality(a, c));
		}

		TEST_METHOD(FooEquality)
		{
			DefaultEquality<Foo> equality;

			Foo a = Foo(10);
			Foo b = Foo(20);
			Foo c(a);

			Assert::IsTrue(equality(a, a));
			Assert::IsFalse(equality(a, b));
			Assert::IsTrue(equality(a, c));
		}

		TEST_METHOD(StringEquality)
		{
			DefaultEquality<std::string> equality;

			std::string a = "Hello"s;
			std::string b = "Goodbye"s;
			std::string c(a);

			Assert::IsTrue(equality(a, a));
			Assert::IsFalse(equality(a, b));
			Assert::IsTrue(equality(a, c));
		}

		TEST_METHOD(WideStringEquality)
		{
			DefaultEquality<std::wstring> equality;

			std::wstring a = L"Hello"s;
			std::wstring b = L"Goodbye"s;
			std::wstring c(a);

			Assert::IsTrue(equality(a, a));
			Assert::IsFalse(equality(a, b));
			Assert::IsTrue(equality(a, c));
		}

		TEST_METHOD(CharacterStringEquality)
		{
			{
				DefaultEquality<char *> equality;
	
				char* a = const_cast<char*>("Hello");
				char* b = const_cast<char*>("Goodbye");
				
				char c[6];
	
	#if defined(_WIN32)
				strcpy_s(c, a);
	#else
				strcpy(c, a);
	#endif
				 
				Assert::IsTrue(equality(a, a));
				Assert::IsFalse(equality(a, b));
				Assert::IsTrue(equality(a, c));
			}

			{
				DefaultEquality<const char *> equality;
	
				const char* a = "Hello";
				const char* b = "Goodbye";
				
				char c[6];
	
	#if defined(_WIN32)
				strcpy_s(c, a);
	#else
				strcpy(c, a);
	#endif
				 
				Assert::IsTrue(equality(a, a));
				Assert::IsFalse(equality(a, b));
				Assert::IsTrue(equality(a, c));
			}

			{
				DefaultEquality<char * const> equality;
	
				char* const a = const_cast<char* const>("Hello");
				char* const b = const_cast<char* const>("Goodbye");
				
				char c[6];
	
	#if defined(_WIN32)
				strcpy_s(c, a);
	#else
				strcpy(c, a);
	#endif
				 
				Assert::IsTrue(equality(a, a));
				Assert::IsFalse(equality(a, b));
				Assert::IsTrue(equality(a, c));
			}

			{
				DefaultEquality<const char * const> equality;
	
				const char* const a = "Hello";
				const char* const b = "Goodbye";
				
				char c[6];
	
	#if defined(_WIN32)
				strcpy_s(c, a);
	#else
				strcpy(c, a);
	#endif
				 
				Assert::IsTrue(equality(a, a));
				Assert::IsFalse(equality(a, b));
				Assert::IsTrue(equality(a, c));
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DefaultEqualityTest::sStartMemState;
}