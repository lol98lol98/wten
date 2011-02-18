#include "stdafx.h"

namespace wten {

using namespace utility;

namespace {

int GetDxLibGraph(const DxLibGraphHandle& handle) {
	return reinterpret_cast<int>(handle.get());
}
void CloseDxLibGraphHandle(const void* handle) {
	const int result = ::DeleteGraph(reinterpret_cast<int>(handle));
	BOOST_ASSERT(result == 0);
}

DxLibGraphHandle IntToDxLibGraphHandle(int handle) {
	return DxLibGraphHandle(reinterpret_cast<void*>(handle), &CloseDxLibGraphHandle);
}

boost::tuple<unsigned int, unsigned int> GetGraphSize(DxLibGraphHandle handle) {
	BOOST_ASSERT(handle);
	int x,y;
	const int result = ::GetGraphSize(GetDxLibGraph(handle), &x, &y);
	BOOST_ASSERT(result != -1);
	return boost::make_tuple<unsigned int, unsigned int>(x, y);
}

unsigned int GetGraphWidth(DxLibGraphHandle handle) {
	return GetGraphSize(handle).get<0>();
}

unsigned int GetGraphHeight(DxLibGraphHandle handle) {
	return GetGraphSize(handle).get<0>();
}

DxLibGraphHandle GetGraphHandle(boost::shared_ptr<const std::string> filename) {
	BOOST_ASSERT(filename);
	BOOST_ASSERT(!filename->empty());
	const int handle = ::LoadGraph(filename->c_str());

	BOOST_ASSERT(handle != -1);
	DxLibGraphHandle result = IntToDxLibGraphHandle(handle);
	return result;
}

} // anonymous

Graph::Graph(DxLibGraphHandle handle) :
	inner_ptr(handle), width(GetGraphWidth(inner_ptr)), height(GetGraphHeight(inner_ptr))
{
	BOOST_ASSERT(handle);
	BOOST_ASSERT(width > 0);
	BOOST_ASSERT(height > 0);
}

Graph::Graph(boost::shared_ptr<const std::string> filename) :
	inner_ptr(GetGraphHandle(filename)), width(GetGraphWidth(inner_ptr)), height(GetGraphHeight(inner_ptr))
{
	BOOST_ASSERT(inner_ptr);
	BOOST_ASSERT(width > 0);
	BOOST_ASSERT(height > 0);
}

unsigned int Graph::GetWidth() const {
	return width;
}

unsigned int Graph::GetHeight() const {
	return height;
}

boost::optional<boost::shared_ptr<Error> > Graph::Draw(unsigned int x, unsigned int y) const {
	BOOST_ASSERT(inner_ptr);
	const int result = ::DrawGraph(x, y, GetDxLibGraph(inner_ptr), TRUE);
	if(result == -1) {
		return CREATE_ERROR(ERROR_CODE_DXLIB_INTERNAL_ERROR);
	}
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > Graph::DrawEx(unsigned int x, unsigned int y, unsigned int w, unsigned int h) const {
	BOOST_ASSERT(inner_ptr);
	const int result = ::DrawExtendGraph(x, y, x + w, y + h, GetDxLibGraph(inner_ptr), TRUE);
	if(result == -1) {
		return CREATE_ERROR(ERROR_CODE_DXLIB_INTERNAL_ERROR);
	}
	return boost::none;
}

opt_error<boost::shared_ptr<Graph> >::type Graph::Derivation(unsigned int x, unsigned int y, unsigned int w, unsigned int h) const {
	BOOST_ASSERT(inner_ptr);
	const int handle = ::DerivationGraph(x, y, w, h, GetDxLibGraph(inner_ptr));
	if(handle == -1) {
		return CREATE_ERROR(ERROR_CODE_DXLIB_INTERNAL_ERROR);
	}
	return boost::shared_ptr<Graph>(new Graph(IntToDxLibGraphHandle(handle)));
}

} // wten

