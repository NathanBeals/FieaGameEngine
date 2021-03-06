#pragma once

// Standard
#include <cstdint>
#include <string>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <algorithm>
#include <sstream>
#include <istream>
#include <fstream>
#include <chrono>
#include <mutex>
#include <future>

// GSL Library
#include <gsl/gsl>
#include <gsl/span>

// OpenGL Mathematics
#pragma warning(disable : 26812)
#include <glm/glm.hpp>
#pragma warning(default : 26812)

#pragma warning(disable : 4201)
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default : 4201)

// JsonCpp
#pragma warning(disable : 26812)
#include <json/json.h>
#pragma warning(default : 26812)