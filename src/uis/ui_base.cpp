#include "stdafx.h"

namespace wten { namespace uis {

using namespace utility;

namespace {

boost::optional<boost::shared_ptr<Error> > OwnerRectCheck(const boost::weak_ptr<windows::WindowBase> owner, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	boost::shared_ptr<windows::WindowBase> window = owner.lock();
	if(!window) {
		return CreateError(ERROR_CODE_INTERNAL_ERROR);
	}

	unsigned int owner_width;
	unsigned int owner_height;
	{
		opt_error<boost::tuple<unsigned int, unsigned int> >::type owner_size = window->GetSize();
		if(owner_size.which() == 0) {
			return boost::get<boost::shared_ptr<Error> >(owner_size);
		}
		boost::tie(owner_width, owner_height) = boost::get<boost::tuple<unsigned int, unsigned int> >(owner_size);
	}

	if(x+width > owner_width || y+height > owner_height) {
		return CreateError(ERROR_CODE_OUTSIDE_RANGE);
	}
	return boost::none;
}

} // anonymous

UIBase::UIBase() :
	x(0),	y(0),	width(0),	height(0)
{
}

UIBase::~UIBase() {
}

boost::optional<boost::shared_ptr<Error> > UIBase::SetOwnerWindow(boost::weak_ptr<windows::WindowBase> window) {
	if(owner.lock()) {
		return CreateError(ERROR_CODE_INTERNAL_ERROR);
	}
	owner = window;
	BOOST_ASSERT(owner.lock());
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > UIBase::InnerRectCheck(void) {
	opt_error<unsigned int>::type width = CalcWidth();
	if(width.which() == 0) {
		return boost::get<boost::shared_ptr<Error> >(width);
	}
	opt_error<unsigned int>::type height = CalcHeight();
	if(height.which() == 0) {
		return boost::get<boost::shared_ptr<Error> >(height);
	}
	if(this->width < boost::get<unsigned int>(width) || this->height < boost::get<unsigned int>(height)) {
		return CreateError(ERROR_CODE_OUTSIDE_RANGE);
	}
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > UIBase::PointAndSizeIsValid(void) {
	boost::optional<boost::shared_ptr<Error> > error;
	if(error = OwnerRectCheck(owner, x, y, width, height)) {
		return error.get();
	}
	if(error = InnerRectCheck()) {
		return error.get();
	}
	return boost::none;
}

opt_error<boost::tuple<unsigned int, unsigned int> >::type UIBase::GetPoint(void) {
	return boost::make_tuple<unsigned int, unsigned int>(x, y);
}

opt_error<boost::tuple<unsigned int, unsigned int> >::type UIBase::GetAbsolutePoint(void) {
	boost::shared_ptr<windows::WindowBase> window = owner.lock();
	if(!window) {
		return CreateError(ERROR_CODE_INTERNAL_ERROR);
	}
	opt_error<boost::tuple<unsigned int, unsigned int> >::type owner_point = window->GetPoint();
	if(owner_point.which() == 0) {
		return boost::get<boost::shared_ptr<Error> >(owner_point);
	}
	unsigned int owner_x;
	unsigned int owner_y;
	boost::tie(owner_x, owner_y) = boost::get<boost::tuple<unsigned int, unsigned int> >(owner_point);
	return boost::make_tuple<unsigned int, unsigned int>(owner_x + x, owner_y + y);
}

opt_error<boost::tuple<unsigned int, unsigned int> >::type UIBase::GetSize(void) {
	return boost::make_tuple<unsigned int, unsigned int>(width, height);
}

boost::optional<boost::shared_ptr<Error> > UIBase::Move(unsigned int x, unsigned int y) {
	this->x = x;
	this->y = y;
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > UIBase::AbsoluteMove(unsigned int x, unsigned int y) {
	boost::shared_ptr<windows::WindowBase> window = owner.lock();
	if(!window) {
		return CreateError(ERROR_CODE_INTERNAL_ERROR);
	}
	opt_error<boost::tuple<unsigned int, unsigned int> >::type owner_point = window->GetPoint();
	if(owner_point.which() == 0) {
		return boost::get<boost::shared_ptr<Error> >(owner_point);
	}
	unsigned int owner_x;
	unsigned int owner_y;
	boost::tie(owner_x, owner_y) = boost::get<boost::tuple<unsigned int, unsigned int> >(owner_point);
	if(x < owner_x || y < owner_y) {
		return CreateError(ERROR_CODE_OUTSIDE_RANGE);
	}
	return Move(x-owner_x, y-owner_y);
}

boost::optional<boost::shared_ptr<Error> > UIBase::Resize(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	return boost::none;
}

boost::optional<boost::shared_ptr<Error> > UIBase::Resize(void) {
	opt_error<unsigned int>::type width = CalcWidth();
	if(width.which() == 0) {
		return boost::get<boost::shared_ptr<Error> >(width);
	}
	opt_error<unsigned int>::type height = CalcHeight();
	if(height.which() == 0) {
		return boost::get<boost::shared_ptr<Error> >(height);
	}
	boost::optional<boost::shared_ptr<Error> > error = Resize(boost::get<unsigned int>(width), boost::get<unsigned int>(height));
	if(error) {
		return error.get();
	}
	return boost::none;
}

utility::opt_error<unsigned int>::type UIBase::CalcWidth() {
	return 0;
}

utility::opt_error<unsigned int>::type UIBase::CalcHeight() {
	return 0;
}

} // uis

} // wten
