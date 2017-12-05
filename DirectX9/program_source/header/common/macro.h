/*
 * <작성 날짜>
 * 2017-12-05
 *
 * <파일 설명>
 * 공용되는 매크로입니다.
 */

#ifndef COMMON_MACRO_H_
#define COMMON_MACRO_H_

#include "std_header.h"

#define MSGBOX_OK(text) MessageBox(nullptr, _T(text), _T("Error"), MB_OK)

#define SAFE_DELETE(ptr)\
	if (ptr)\
	{\
		delete ptr;\
		ptr = nullptr;\
	}\

#define SAFE_RELEASE(ptr)\
	if (ptr)\
	{\
		ptr->Release();\
		ptr = nullptr;\
	}\

#define DEFINE_SINGLETON_CLASS(ClassType)\
	public:\
		static ClassType& ObtainInstance()\
		{\
			if (m_pInst == nullptr)\
			{\
				std::once_flag flag;\
				std::function<void()> alloc = [](){ m_pInst.reset(new ClassType()); };\
				std::call_once(flag, alloc);\
			}\
			return *m_pInst;\
		}\
	private:\
		struct Deleter\
		{\
			void operator() (ClassType* ptr)\
			{\
				if (ptr)\
				{\
					ptr->~ClassType();\
					SAFE_DELETE(ptr);\
				}\
			}\
		};\
		\
		ClassType()  = default;\
		~ClassType() = default;\
		ClassType(const ClassType& rhs)  = delete;\
		ClassType(const ClassType&& rhs) = delete;\
		ClassType& operator= (const ClassType& rhs)  = delete;\
		ClassType& operator= (const ClassType&& rhs) = delete;\
		\
		using UniqueSingletonPTR = std::unique_ptr<ClassType, Deleter>;\
		static UniqueSingletonPTR m_pInst;

#define INIT_SINGLETON_INSTANCE(ClassType) std::unique_ptr<ClassType, ClassType::Deleter> ClassType::m_pInst = nullptr

#define GET_SINGLE(ClassType) ClassType::ObtainInstance()

#endif