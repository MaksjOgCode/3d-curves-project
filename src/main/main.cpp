#include <vector>
#include <random>
#include <type_traits>
#include <memory>
#include <iostream>
#include <algorithm>

#include <curves/curves_3d.hpp>



namespace generator {

    inline thread_local std::mt19937_64 generator{ std::random_device{}() };

    template <typename>
    inline constexpr bool always_false_v = false;

    inline double uniform(double lo_, double hi_) {
        return std::uniform_real_distribution <double> { lo_, hi_ }(generator);
    }

    inline constexpr double min_radius = 0.1;
    inline constexpr double max_radius = 10.0;
    inline constexpr double max_pitch  = 5.0;

    template <typename T>
    [[nodiscard]] T create_curve() {
        static_assert (
            std::is_base_of_v <curves_3d::curve_3d, T>,
            "T must be derived from curves_3d::curve_3d"
        );
 
        if constexpr ( std::is_same_v <T, curves_3d::circle_3d> ) {
            return curves_3d::circle_3d::create (
                uniform(min_radius, max_radius)
            ).value();
        }
        else if constexpr ( std::is_same_v <T, curves_3d::ellipse_3d> ) {
            return curves_3d::ellipse_3d::create (
                uniform(min_radius, max_radius),
                uniform(min_radius, max_radius)
            ).value();
        }
        else if constexpr ( std::is_same_v <T, curves_3d::helix_3d> ) {
            return curves_3d::helix_3d::create (
                uniform(min_radius, max_radius),
                uniform(min_radius, max_pitch)
            ).value();
        }
        else {
            static_assert (
                always_false_v <T>,
                "Unsupported curve type in rnd::create_curve<T>() "
                "(supported: circle_3d, ellipse_3d, helix_3d)"
            );
        }
    }

    template <typename T>
    [[nodiscard]] std::shared_ptr <curves_3d::curve_3d> create_curve_ptr() {
        return std::make_shared <T> (create_curve <T>());
    }
}



int main() {
    /* Random generation of polymorphic objects: */
    std::vector < std::shared_ptr <curves_3d::curve_3d> > random_curves{};

    /* Circles: */
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::circle_3d >() );
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::circle_3d >() );
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::circle_3d >() );
    /* Ellipses: */
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::ellipse_3d >() );
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::ellipse_3d >() );
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::ellipse_3d >() );
    /* Helixs: */
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::helix_3d >() );
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::helix_3d >() );
    random_curves.emplace_back ( generator::create_curve_ptr < curves_3d::helix_3d >() );



    /* Output point(t) and derivative(t) (t = PI / 4): */
    std::cout << "All curves:\n";
    for ( const auto &curve : random_curves ) {
        std::cout <<
            "Name class: " << curve->name_class() << "\n" <<
            "point(t = PI / 4) = " << curve->point( std::numbers::pi / 4 ) << "\n" <<
            "derivative(t = PI / 4) = " << curve->derivative( std::numbers::pi / 4 ) << "\n\n";
    }



    /* The second container has only the circles from the first container: */
    std::vector < std::weak_ptr <curves_3d::circle_3d> > circles{};
    for (const auto &curve : random_curves ) {
        if ( auto pointer = std::dynamic_pointer_cast <curves_3d::circle_3d> (curve) )
            circles.emplace_back(pointer);
    }
    
    std::cout << "\n\n\nSecond container (only circles):\n";
    for ( const auto& circle_ptr : circles ) {
        if ( auto circle = circle_ptr.lock() ) {
            std::cout <<
                "Name class: " << circle->name_class() << "\n" <<
                "point(t = PI / 4) = " << circle->point( std::numbers::pi / 4 ) << "\n" <<
                "derivative(t = PI / 4) = " << circle->derivative( std::numbers::pi / 4 ) << "\n\n";
        }
    }



    /* Sorting the second container from a smaller radius to a larger radius: */
    std::ranges::sort (
        circles,
        [] ( const std::weak_ptr <curves_3d::circle_3d>& a,
             const std::weak_ptr <curves_3d::circle_3d>& b ) -> bool
        {
            auto ptr_a = a.lock(), ptr_b = b.lock();
            return ptr_a->radius() < ptr_b->radius();
        }
    );
    std::cout << "\n\n\nSorted second container (only circles):\n";
    for ( const auto& circle_ptr : circles ) {
        if ( auto circle = circle_ptr.lock() ) {
            std::cout <<
                "Name class: " << circle->name_class() << "\n" <<
                "radius = " << circle->radius() << "\n" <<
                "point(t = PI / 4) = " << circle->point( std::numbers::pi / 4 ) << "\n" <<
                "derivative(t = PI / 4) = " << circle->derivative( std::numbers::pi / 4 ) << "\n\n";
        }
    }

    

    /* The total value of the radii of all curves from the second container: */
    double total_radius = (
        std::accumulate(
            circles.begin(),
            circles.end(),
            0.0,
            [](double sum, std::weak_ptr <curves_3d::circle_3d>& ptr_circle) -> double {
                if (auto circle = ptr_circle.lock() ) { return sum + circle->radius(); }

                return sum;
            }
        )
    );
    std::cout << "\n\n\nTotal radius circles from second container: " << total_radius << "\n\n";

    return 0;
}