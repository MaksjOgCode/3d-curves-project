/**
 * @file   curves_3d.hpp
 * @author github: MaksjOgCode
 * @brief  Library of simple 3D parametric curves.
 *
 * @details
 * Provides a polymorphic hierarchy of parametric curves in 3D space with a
 * unified interface for evaluating:
 * - a point on the curve at parameter @p t
 * - the first derivative (tangent vector) at parameter @p t
 *
 * The current implementation contains:
 * - `circle_3d`  (in the XY plane)
 * - `ellipse_3d` (in the XY plane)
 * - `helix_3d`   (around the Z axis)
 *
 * Factory methods return `std::expected <T, curve_3d_error>` instead of throwing
 * exceptions, allowing explicit error handling for invalid parameters.
 *
 * @note Parameter `t` is interpreted in radians for trigonometric curves.
 * @note Requires C++23 for `std::expected`.
 *
 * @date    2026-02-23
 * @version 0.1.0
 */
#ifndef CURVES_3D_HPP
#define CURVES_3D_HPP



#include <string>
#include <expected>
#include <cstdint>



/// Main namespace of the 3D curves library.
namespace curves_3d {

/**
 * @brief Simple 3D vector/point container.
 *
 * @details
 * This type is used both as:
 * - a point in 3D space (for curve evaluation), and
 * - a derivative/tangent vector (for first derivative evaluation).
 */
struct vector_3d {
    double _x {};
    double _y {};
    double _z {};
};

/**
 * @brief Overloading the output operator for vector_3d to the std::ostream stream.
 * 
 * Outputs a three-dimensional vector in the (x,y,z) format.
 * 
 * @param output_stream_ Target output stream (std::ostream&).
 * @param v_             vector_3d for output.
 * @return               Ready object to be transferred to the output stream
 * 
 * @example
 * @code
 * curves_3d::vector_3d pos{1.0, 2.5, -3.14};
 * std::cout << pos << std::endl;  // Output: (1, 2.5, -3.14)
 * @endcode
 */
std::ostream& operator<<( std::ostream& output_stream_, const vector_3d& v_ );



/**
 * @brief Error codes used by curve factory methods.
 *
 * @details
 * All static `create(...)` methods validate input parameters and return an
 * error code in `std::unexpected(...)` if validation fails.
 */
enum class curve_3d_error : std::uint8_t {
    /// Invalid circle radius (must be strictly positive).
    bad_radius_circle_3d,

    /// Invalid ellipse radii (intended: both radii must be strictly positive).
    bad_radii_ellipse_3d,

    /// Invalid helix parameters (radius and pitch must be strictly positive).
    bad_helix_3d
};

/**
 * @brief Converts a curve error code to a human-readable message.
 *
 * @param error_code_ Error code.
 * @return            std::string Human-readable description of the error.
 */
std::string str_curve_3d_error(curve_3d_error error_code_);



/**
 * @brief Abstract base class for a 3D parametric curve.
 *
 * @details
 * Defines a minimal interface for evaluating a parametric curve:
 * \f[
 *   \mathbf{r}(t) = (x(t), y(t), z(t))
 * \f]
 * and its first derivative:
 * \f[
 *   \mathbf{r}'(t)
 * \f]
 *
 * Derived classes must implement both methods.
 */
class curve_3d {
public:
    /// Virtual destructor for safe polymorphic use.
    virtual ~curve_3d() = default;

    /**
     * @brief Evaluates a point on the curve.
     *
     * @param t_ Curve parameter (typically radians).
     * @return   `vector_3d` point \f$\mathbf{r}(t)\f$ on the curve.
     */
    virtual vector_3d point(double t_) const = 0;

    /**
     * @brief Evaluates the first derivative of the curve.
     *
     * @param t_ Curve parameter (typically radians).
     * @return   `vector_3d` tangent vector \f$\mathbf{r}'(t)\f$.
     */
    virtual vector_3d derivative(double t_) const = 0;

    /**
     * @brief  Returns the class name.
     *
     * @return Returns a constant string as a class name
     */
    virtual std::string name_class() const = 0;
};



/**
 * @brief Circle in the XY plane centered at the origin.
 *
 * @details
 * Parametric form:
 * \f[
 *   x(t)=R\cos(t), \quad y(t)=R\sin(t), \quad z(t)=0
 * \f]
 *
 * First derivative:
 * \f[
 *   x'(t)=-R\sin(t), \quad y'(t)=R\cos(t), \quad z'(t)=0
 * \f]
 */
class circle_3d final : public curve_3d {
public:
    /**
     * @brief Creates a circle with the given radius.
     *
     * @param radius_ Circle radius. Must be strictly positive.
     * 
     * @return `std::expected <circle_3d, curve_3d_error>`
     *         - circle_3d on success
     *         - `curve_3d_error::bad_radius_circle_3d` on failure
     */
    static std::expected <circle_3d, curve_3d_error> create(double radius_) noexcept;

    /**
     * @copydoc curve_3d::point
     */
    vector_3d point(double t_) const noexcept override;

    /**
     * @copydoc curve_3d::derivative
     */
    vector_3d derivative(double t_) const noexcept override;

    /**
     * @copydoc curve_3d::name_class
     */
    std::string name_class() const noexcept override;

    /**
     * @brief Returns the radius of the circle of the current object:
     * 
     * @return double radius. 
     */
    double radius() const noexcept;
private:
    /**
     * @brief Constructs a circle without validation.
     * 
     * @param radius_ Valid radius (> 0).
     */
    explicit circle_3d(double radius_) noexcept
        : _radius(radius_) {}

    /// Circle radius (strictly positive).
    double _radius {};
};



/**
 * @brief Ellipse in the XY plane centered at the origin.
 *
 * @details
 * Parametric form:
 * \f[
 *   x(t)=R_x\cos(t), \quad y(t)=R_y\sin(t), \quad z(t)=0
 * \f]
 *
 * First derivative:
 * \f[
 *   x'(t)=-R_x\sin(t), \quad y'(t)=R_y\cos(t), \quad z'(t)=0
 * \f]
 *
 * @note The object is disabled when `rx_ <= 0 && ry_ <= 0`.
 */
class ellipse_3d final : public curve_3d {
public:
    /**
     * @brief Creates an ellipse with the given radii.
     *
     * @param rx_ Radius along the X axis. Intended to be strictly positive.
     * @param ry_ Radius along the Y axis. Intended to be strictly positive.
     * 
     * @return `std::expected <ellipse_3d, curve_3d_error>`
     *         - ellipse_3d on success
     *         - `curve_3d_error::bad_radii_ellipse_3d` on failure
     */
    static std::expected <ellipse_3d, curve_3d_error> create(double rx_, double ry_) noexcept;

    /**
     * @copydoc curve_3d::point
     */
    vector_3d point(double t_) const noexcept override;

    /**
     * @copydoc curve_3d::derivative
     */
    vector_3d derivative(double t_) const noexcept override;

    /**
     * @copydoc curve_3d::name_class
     */
    std::string name_class() const noexcept override;
private:
    /**
     * @brief Constructs an ellipse without validation.
     * @param rx_ X radius (valid x > 0).
     * @param ry_ Y radius (valid x > 0).
     */
    ellipse_3d(double rx_, double ry_)
        : _rx(rx_), _ry(ry_) { }

    /// Radius along the X axis.
    double _rx {};
    
    /// Radius along the Y axis.
    double _ry {};
};



/**
 * @brief Circular helix around the Z axis.
 *
 * @details
 * The helix uses:
 * - radius in the XY plane
 * - pitch per full turn (increase in Z over \f$2\pi\f$ in parameter)
 *
 * Parametric form:
 * \f[
 *   x(t)=R\cos(t), \quad y(t)=R\sin(t), \quad z(t)=\frac{P}{2\pi}t
 * \f]
 * where \f$P\f$ is the pitch per turn.
 *
 * First derivative:
 * \f[
 *   x'(t)=-R\sin(t), \quad y'(t)=R\cos(t), \quad z'(t)=\frac{P}{2\pi}
 * \f]
 */
class helix_3d final : public curve_3d {
public:
    /**
     * @brief Creates a helix with the given radius and pitch.
     *
     * @param radius_         Helix radius in the XY plane. Must be strictly positive.
     * @param pitch_per_turn_ Helix pitch per full turn (along Z). Must be strictly positive.
     * 
     * @return `std::expected<helix_3d, curve_3d_error>`
     *         - helix_3d on success
     *         - `curve_3d_error::bad_helix_3d` on failure
     */
    static std::expected <helix_3d, curve_3d_error> create(double radius_, double pitch_per_turn_) noexcept;

    /**
     * @copydoc curve_3d::point
     */
    vector_3d point(double t_) const noexcept override;

    /**
     * @copydoc curve_3d::derivative
     */
    vector_3d derivative(double t_) const noexcept override;

    /**
     * @copydoc curve_3d::name_class
     */
    std::string name_class() const noexcept override;
private:
    /**
     * @brief Constructs a helix without validation.
     * 
     * @param radius_         Valid radius (> 0).
     * @param pitch_per_turn_ Valid pitch per turn (> 0).
     */
    explicit helix_3d(double radius_, double pitch_per_turn_)
        : _radius(radius_), _pitch(pitch_per_turn_) { }

    /// Helix radius in the XY plane.
    double _radius;

    /// Helix pitch per full turn (Z increment over one revolution).
    double _pitch;
};
}; // namespace curves_3d



#endif /* !CURVES_3D_HPP */ 