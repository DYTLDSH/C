/**
 * File              : dynamic/dynamic.h
 * Author            : huxiao <huxiao@bytedance.com>
 * Date              : 2019/02/01
 * Last Modified Date: 2019/02/01
 * Last Modified By  : huxiao <huxiao@bytedance.com>
 */
#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <cxxabi.h>

namespace cpputil {
namespace dynamic {

template <typename... Args>
void PrintArgs(const char* desc, Args&&... args) {
  int argn = sizeof...(Args);
  const char* argc[] = {typeid(Args).name()...};
  std::cout << desc << std::endl;
  for (int i = 0; i < argn; i++) {
    char* tpe = abi::__cxa_demangle(argc[i], nullptr, nullptr, nullptr);
    std::cout << "[" << tpe << "]";
    free(tpe);
  }
  std::cout << std::endl;
}

template <typename T, typename... Args>
class DynamicFactory {
 private:
  DynamicFactory() = default;
  DynamicFactory(DynamicFactory&) = delete;
  DynamicFactory& operator=(DynamicFactory&) = delete;

 public:
  virtual ~DynamicFactory() = default;

  static DynamicFactory* Instance() {
    static std::unique_ptr<DynamicFactory<T, Args...>> instance(
        new DynamicFactory<T, Args...>());
    return instance.get();
  }

  int regist(const std::string& ttype,
             std::function<std::shared_ptr<T>(Args&&... args)> func) {
    bool status = func_map_.emplace(ttype, func).second;
    if (!status) {
      std::cerr << "regist ttype [" << ttype << "] failed." << std::endl;
      return -2;
    }
    bool verbos = true;
    if (verbos) {
      int argn = sizeof...(Args);
      const char* argc[] = {typeid(Args).name()...};
      std::cout << "Register dynamic_clazz: " << ttype << " with args:";
      for (int i = 0; i < argn; i++) {
        char* tpe = abi::__cxa_demangle(argc[i], nullptr, nullptr, nullptr);
        std::cout << "[" << tpe << "]";
        free(tpe);
      }
      std::cout << ", dynamic_size=" << func_map_.size() << std::endl;
    }
    return 0;
  }

  int regist_stateless(const std::string& ttype,
                       std::shared_ptr<T>&& stateless_instance) {
    bool status = stateless_map_.emplace(ttype, stateless_instance).second;
    if (!status) {
      std::cerr << "regist ttype [" << ttype << "] failed." << std::endl;
      return -2;
    }
    bool verbos = true;
    if (verbos) {
      int argn = sizeof...(Args);
      const char* argc[] = {typeid(Args).name()...};
      std::cout << "Register stateless_clazz: " << ttype << " with args:";
      for (int i = 0; i < argn; i++) {
        char* tpe = abi::__cxa_demangle(argc[i], nullptr, nullptr, nullptr);
        std::cout << "[" << tpe << "]";
        free(tpe);
      }
      std::cout << ", stateless_size=" << stateless_map_.size() << std::endl;
    }
    return 0;
  }

  std::shared_ptr<T> create(bool verbose, const std::string& ttype,
                            Args&&... args) {
    auto iter = func_map_.find(ttype);
    if (iter == func_map_.end()) {
      if (verbose) {
        std::cerr << "failed to find dynamic ttype=[" << ttype << "]"
                  << std::endl;
      }
      return nullptr;
    } else {
      return iter->second(std::forward<Args>(args)...);
    }
  }

  std::shared_ptr<T> get_stateless(bool verbose, const std::string& ttype) {
    auto iter = stateless_map_.find(ttype);
    if (iter == stateless_map_.end()) {
      if (verbose) {
        std::cerr << "failed to find stateless ttype=[" << ttype << "]"
                  << std::endl;
      }
      return nullptr;
    } else {
      return iter->second;
    }
  }

 private:
  std::unordered_map<std::string, std::function<std::shared_ptr<T>(Args&&...)>>
      func_map_;
  std::unordered_map<std::string, std::shared_ptr<T>> stateless_map_;
};

template <typename T1, typename T2, typename... Args>
class DynamicCreator {
 public:
  DynamicCreator() {
    register_.ping();
  }
  virtual ~DynamicCreator() = default;

  static std::shared_ptr<T2> CreateObject(Args&&... args) {
    return std::make_shared<T2>(std::forward<Args>(args)...);
  }

  static std::string get_demangle_name() {
    char* demangle_name =
        abi::__cxa_demangle(typeid(T2).name(), nullptr, nullptr, nullptr);
    if (demangle_name != nullptr) {
      std::string ret(demangle_name);
      free(demangle_name);
      return ret;
    }
    return "unknown";
  }

  struct Register {
    Register() {
      char* demangle_name =
          abi::__cxa_demangle(typeid(T2).name(), nullptr, nullptr, nullptr);
      if (demangle_name != nullptr) {
#if defined(__aarch64__) || __GNUC__ >= 7 || defined(__clang__)
        std::string _demangle_name(demangle_name);
        std::string _source(
            "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > ");
        std::string _source2(
            "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >");
        std::string _target("std::string");
        std::size_t pos = std::string::npos;
        while ((pos = _demangle_name.find(_source)) != std::string::npos) {
          _demangle_name = _demangle_name.replace(
              pos, _source.size(), _target.c_str(), _target.size());
        }
        while ((pos = _demangle_name.find(_source2)) != std::string::npos) {
          _demangle_name = _demangle_name.replace(
              pos, _source2.size(), _target.c_str(), _target.size());
        }
        DynamicFactory<T1, Args...>::Instance()->regist(
            _demangle_name.c_str(), DynamicCreator::CreateObject);
#else
        DynamicFactory<T1, Args...>::Instance()->regist(
            demangle_name, DynamicCreator::CreateObject);
#endif

        free(demangle_name);
      }
    }

    inline void ping() const {
    }
  };

 private:
  static Register register_;
};

template <typename T1, typename T2, typename... Args>
typename DynamicCreator<T1, T2, Args...>::Register
    DynamicCreator<T1, T2, Args...>::register_;

template <typename T1, typename T2>
class StatelessCreator {
 public:
  StatelessCreator() {
    register_.ping();
  }
  virtual ~StatelessCreator() = default;

  static std::string get_demangle_name() {
    char* demangle_name =
        abi::__cxa_demangle(typeid(T2).name(), nullptr, nullptr, nullptr);
    if (demangle_name != nullptr) {
      std::string ret(demangle_name);
      free(demangle_name);
      return ret;
    }
    return "unknown";
  }

  struct Register {
    Register() {
      char* demangle_name =
          abi::__cxa_demangle(typeid(T2).name(), nullptr, nullptr, nullptr);
      if (demangle_name != nullptr) {
#if defined(__aarch64__) || __GNUC__ >= 7 || defined(__clang__)
        std::string _demangle_name(demangle_name);
        std::string _source(
            "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > ");
        std::string _source2(
            "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >");
        std::string _target("std::string");
        std::size_t pos = std::string::npos;
        while ((pos = _demangle_name.find(_source)) != std::string::npos) {
          _demangle_name = _demangle_name.replace(
              pos, _source.size(), _target.c_str(), _target.size());
        }
        while ((pos = _demangle_name.find(_source2)) != std::string::npos) {
          _demangle_name = _demangle_name.replace(
              pos, _source2.size(), _target.c_str(), _target.size());
        }

        DynamicFactory<T1>::Instance()->regist_stateless(
            _demangle_name.c_str(), std::make_shared<T2>());
#else
        DynamicFactory<T1>::Instance()->regist_stateless(
            demangle_name, std::make_shared<T2>());
#endif
        free(demangle_name);
      }
    }

    inline void ping() const {
    }
  };

 private:
  static Register register_;
};

template <typename T1, typename T2>
typename StatelessCreator<T1, T2>::Register StatelessCreator<T1, T2>::register_;

template <typename T>
class DynamicWorker {
 public:
  DynamicWorker() = default;
  DynamicWorker(bool verbose) : verbose_(verbose) {
  }
  virtual ~DynamicWorker() = default;

 public:
  template <typename... Args>
  std::shared_ptr<T> CreateDynamic(const std::string& ttype, Args&&... args) {
    auto p = DynamicFactory<T, Args...>::Instance()->create(
        verbose_, ttype, std::forward<Args>(args)...);
    return p;
  }

  std::shared_ptr<T> GetStateless(const std::string& ttype) {
    auto p = DynamicFactory<T>::Instance()->get_stateless(verbose_, ttype);
    return p;
  }

 private:
  bool verbose_ = true;
};

}  // namespace dynamic
}  // namespace cpputil
