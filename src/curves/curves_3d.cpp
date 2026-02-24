/**
 * @file  curves_3d.cpp
 * @brief Implementation of the 3D parametric curves library.
 */
#include <numbers>
#include <cmath>
#include <iostream>

#include "curves_3d.hpp"



namespace curves_3d {

std::ostream& operator<<( std::ostream& output_stream_, const vector_3d& v_ ) {
    output_stream_ << '(' << v_._x << ", " << v_._y << ", " << v_._z << ')';
    return output_stream_;
}



std::string str_curve_3d_error(curve_3d_error error_code_) {
    switch ( error_code_ ) {
        case curve_3d_error::bad_radius_circle_3d:
            return "circle radius must be positive (radius > 0)";

        case curve_3d_error::bad_radii_ellipse_3d:
            return "ellipse radii must be positive (rx > 0 && ry > 0)";

        case curve_3d_error::bad_helix_3d:
            return "helix3d radius and pitch must be positive (radius > 0 && pitch > 0)";

        default: 
            return "unknown error";
    }
}



/* class: circle_3d */
std::expected <circle_3d, curve_3d_error> circle_3d::create(double radius_) noexcept {
    if (radius_ > 0.0) [[likely]]
        return circle_3d(radius_);
    else [[unlikely]]
        return std::unexpected(curve_3d_error::bad_radius_circle_3d);
}

vector_3d circle_3d::point(double t_) const noexcept {
    return { // RVO
        ._x = ( (this->_radius) * std::cos(t_) ),
        ._y = ( (this->_radius) * std::sin(t_) ),
        ._z = ( double(0.0) )
    };
}

vector_3d circle_3d::derivative(double t_) const noexcept {
    return { // RVO
        ._x = ( -(this->_radius) * std::sin(t_) ),
        ._y = (  (this->_radius) * std::cos(t_) ),
        ._z = ( double(0.0) )
    };
}

std::string circle_3d::name_class() const noexcept {
    return "circle_3d";
}

double circle_3d::radius() const noexcept {
    return this->_radius;
}



/* class: ellipse_3d */
std::expected <ellipse_3d, curve_3d_error> ellipse_3d::create(double rx_, double ry_) noexcept {
    if ( rx_ > double(0.0) && ry_ > double(0.0) ) [[likely]]
        return ellipse_3d(rx_, ry_);
    else [[unlikely]]
        return std::unexpected(curve_3d_error::bad_radii_ellipse_3d);
}

vector_3d ellipse_3d::point(double t_) const noexcept {
    return { // RVO
        ._x = ( (this->_rx) * std::cos(t_) ),
        ._y = ( (this->_ry) * std::sin(t_) ),
        ._z = ( double(0.0) )
    };
}

vector_3d ellipse_3d::derivative(double t_) const noexcept {
    return { // RVO
        ._x = ( -(this->_rx) * std::sin(t_) ),
        ._y = (  (this->_ry) * std::cos(t_) ),
        ._z = ( double(0.0) )
    };
}

std::string ellipse_3d::name_class() const noexcept {
    return "ellipse_3d";
}



/* class: helix_3d: */
std::expected <helix_3d, curve_3d_error> helix_3d::create(double radius_, double pitch_per_turn_) noexcept {
    if ( radius_ > 0.0 && pitch_per_turn_ > 0.0 ) [[likely]]
        return helix_3d(radius_, pitch_per_turn_);
    else [[unlikely]]
        return std::unexpected(curve_3d_error::bad_helix_3d);
}

vector_3d helix_3d::point(double t_) const noexcept
{
    return { // RVO
        ._x = (  (this->_radius) * std::cos(t_) ),
        ._y = (  (this->_radius) * std::sin(t_) ),
        ._z = ( ((this->_pitch) / (2.0 * std::numbers::pi)) * t_ )
    };
}

vector_3d helix_3d::derivative(double t_) const noexcept
{
    return { // RVO
        ._x = ( -(this->_radius) * std::sin(t_) ),
        ._y = (  (this->_radius) * std::cos(t_) ),
        ._z = (  (this->_pitch) / (2.0 * std::numbers::pi)  )
    };
}

std::string helix_3d::name_class() const noexcept {
    return "helix_3d";
}
}; /* !namespace curves_3d */