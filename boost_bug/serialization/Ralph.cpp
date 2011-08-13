#include <sstream>

#include <boost/serialization/export.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

class A {
public:
   template<class Archive>
   void serialize(Archive & ar, const unsigned version)
   {
   }
};

BOOST_CLASS_EXPORT(A)

int main()
{
   A a;

   std::ostringstream os;
   //boost::archive::polymorphic_binary_oarchive oa(os);
   boost::archive::binary_oarchive oa(os);
   oa & a;
}
