#include "pch.h"

#include "ToStringSpecialization.h"
#include "JsonParseMaster.h"
#include "JsonParseTestHelper.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseTest)
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

		TEST_METHOD(Move)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			JsonParseMaster moveConstructed(std::move(parser));
			Assert::IsNull(parser.GetSharedData());

			{
				const JsonParseMaster::SharedData* sharedDataPtr = &sharedData;
				Assert::AreEqual(sharedDataPtr, moveConstructed.GetSharedData());
			}

			moveConstructed.Parse(R"({ "mInt": 10 })");

			Assert::AreEqual(1_z, sharedData.GetSize("mInt"));

			JsonParseMaster moveAssigned;
			moveAssigned = std::move(moveConstructed);
			Assert::IsNull(parser.GetSharedData());
			Assert::IsNull(moveConstructed.GetSharedData());

			{
				const JsonParseMaster::SharedData* sharedDataPtr = &sharedData;
				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
			}

			moveAssigned.Parse(R"({ "mInt": 10 })");

			Assert::AreEqual(1_z, sharedData.GetSize("mInt"));

			JsonParseTestHelper::SharedData moveConstructedSharedData(std::move(sharedData));
			{
				const JsonParseMaster::SharedData* sharedDataPtr = &moveConstructedSharedData;
				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
			}

			JsonParseTestHelper::SharedData moveAssignedSharedData;
			moveAssignedSharedData = std::move(moveConstructedSharedData);
			{
				const JsonParseMaster::SharedData* sharedDataPtr = &moveAssignedSharedData;
				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
			}
		}

		TEST_METHOD(Accessors)
		{
			JsonParseTestHelper::SharedData sharedData;

			JsonParseTestHelper helper;
			JsonParseMaster parser;

			Assert::IsNull(sharedData.GetJsonParseMaster());
			Assert::IsNull(parser.GetSharedData());
			parser.SetSharedData(sharedData);
			Assert::IsNotNull(sharedData.GetJsonParseMaster());
			Assert::IsNotNull(parser.GetSharedData());			

			{
				const JsonParseTestHelper::SharedData constSharedData;
				Assert::IsNull(constSharedData.GetJsonParseMaster());
			}
			{
				const JsonParseTestHelper::SharedData constSharedData = sharedData;
				Assert::AreEqual(sharedData.GetJsonParseMaster(), constSharedData.GetJsonParseMaster());
			}

			const JsonParseMaster constParser;
			Assert::IsNull(constParser.GetSharedData());
		}

		TEST_METHOD(Modifiers)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mInt": 10 })");

			Assert::AreEqual(0_z, sharedData.GetSize("mInt"));

			parser.AddHelper(helper);
			Assert::ExpectException<std::runtime_error>([&parser, &helper] { parser.AddHelper(helper); });

			parser.Parse(R"({ "mInt": 10, "null": null })");

			Assert::AreEqual(1_z, sharedData.GetSize("mInt"));

			Assert::IsTrue(parser.RemoveHelper(helper));

			parser.Parse(R"({ "mInt": 10 })");

			Assert::AreEqual(0_z, sharedData.GetSize("mInt"));

			JsonParseTestHelper::SharedData sharedData2;
			parser.SetSharedData(sharedData2);
			Assert::IsNull(sharedData.GetJsonParseMaster());
		}

		TEST_METHOD(Clone)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

 			parser.AddHelper(helper);
 			parser.SetSharedData(sharedData);

			JsonParseMaster* clone = parser.Clone();

 			Assert::IsNotNull(clone->GetSharedData());
 
 			clone->Parse(R"({ "mInt": 10 })");

 			Assert::AreEqual(1_z, clone->GetSharedData()->As<JsonParseTestHelper::SharedData>()->GetSize("mInt"));

			delete clone;
		}

		TEST_METHOD(ParsePrimitives)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10" })");

			Assert::AreEqual(1_z, sharedData.GetSize("mInt"));
			Assert::AreEqual(1_z, sharedData.GetSize("mFloat"));
			Assert::AreEqual(1_z, sharedData.GetSize("mString"));

			std::string filename("JsonTest.json");
			std::ofstream fileTest;
			fileTest.open(filename);
			fileTest << R"({ "mInt": 10, "mFloat": 10, "mString": "10" })";
			fileTest.close();

			Assert::AreEqual(std::string(), parser.GetFilename());
			
			parser.ParseFromFile(filename);

			Assert::AreEqual(filename, parser.GetFilename());
			std::remove(filename.c_str());

			Assert::AreEqual(1_z, sharedData.GetSize("mInt"));
			Assert::AreEqual(1_z, sharedData.GetSize("mFloat"));
			Assert::AreEqual(1_z, sharedData.GetSize("mString"));
		}

		TEST_METHOD(ParseArray)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "array": [1,2,3,4,5] })");
			Assert::AreEqual(5_z, sharedData.GetSize("array"));

			parser.Parse(R"({ "array": [ { "int": 10 }, { "float": 10.0 }, { "string": "10" } ] })");
			Assert::AreEqual(3_z, sharedData.GetSize("array"));

			parser.Parse(R"({ "array": [[[1,2]], [[2,3,4]], [[3,4]]] })");
			Assert::AreEqual(7_z, sharedData.GetSize("array"));

			parser.Parse(R"({ "array": [ { "int": [10,20] }, { "float": [10.0,20.0] }, { "string": ["10","20","30"] } ] })");
			Assert::AreEqual(7_z, sharedData.GetSize("array"));
		}

		TEST_METHOD(ParseObject)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mObject": { "member1": 1, "member2": [2, 3], "member3": "hello", "member4": { "nestedObject1": 1, "nestedObject2": [1,2] } } })");

			Assert::AreEqual(7_z, sharedData.GetSize("mObject"));
		}

		TEST_METHOD(RTTITest)
		{
			std::shared_ptr<JsonParseTestHelper> derivedHelper = std::make_shared<JsonParseTestHelper>();

			Assert::IsTrue(derivedHelper->Is(IJsonParseHelper::TypeIdClass()));
			Assert::IsNotNull(derivedHelper->As<JsonParseTestHelper>());

			std::shared_ptr<IJsonParseHelper> helper = derivedHelper;
			Assert::IsTrue(helper->Is(JsonParseTestHelper::TypeIdClass()));
			Assert::IsNotNull(helper->As<JsonParseTestHelper>());
			
			std::shared_ptr<JsonParseTestHelper::SharedData> derivedSharedData = std::make_shared<JsonParseTestHelper::SharedData>();

			Assert::IsTrue(derivedSharedData->Is(JsonParseMaster::SharedData::TypeIdClass()));
			Assert::IsNotNull(derivedSharedData->As<JsonParseMaster::SharedData>());

			std::shared_ptr<JsonParseMaster::SharedData> sharedData = derivedSharedData;

			Assert::IsTrue(sharedData->Is(JsonParseTestHelper::SharedData::TypeIdClass()));
			Assert::IsNotNull(sharedData->As<JsonParseTestHelper::SharedData>());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParseTest::sStartMemState;
}