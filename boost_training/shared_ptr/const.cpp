#include <boost/shared_ptr.hpp>
#include <vector>

struct My {
	void method() {}
	void const_method() const {}
};

struct MyDerived:My {
};

typedef boost::shared_ptr<My> MySp;
typedef boost::shared_ptr<My const> MyConstSp;

typedef boost::shared_ptr<MyDerived> MyDerivedSp;
typedef boost::shared_ptr<MyDerived const> MyDerivedConstSp;

void foo(MySp msp) {
	msp->method();
	msp->const_method();
}

void foo_cref(MySp const& msp) {
	msp->method();        // OK
	msp->const_method();  
}

void foo_const_sp(MyConstSp msp) {
	// msp->method(); // NG
	msp->const_method();
}

void foo_const_sp_cref(MyConstSp const& msp) {
	// msp->method(); // NG
	msp->const_method();
}

typedef std::vector<MySp> MySpCol;
typedef std::vector<MyConstSp> MyConstSpCol;

template<class T> class const_vector;

template<class T>
class const_vector<boost::shared_ptr<T> >:public std::vector<boost::shared_ptr<T const> > {
public:
	const_vector() {}
	const_vector(std::vector<boost::shared_ptr<T> > const& other):std::vector<boost::shared_ptr<T const> >(other.begin(), other.end()) {}
};

typedef const_vector<MySp> MySpConstCol;

int main() {
	{
		MySp msp(new My);
		MyConstSp mcsp(msp);
		
		foo(msp);
		foo_cref(msp);
		foo_const_sp(msp);
		foo_const_sp_cref(msp);

		// foo(mcsp);  // NG
		// foo_cref(mcsp); // NG
		foo_const_sp(mcsp);
		foo_const_sp_cref(mcsp);

		MySpCol mspcol;
		mspcol.push_back(msp);
		// mspcol.push_back(mcsp); // NG
		// MyConstSpCol mcspcol(mspcol);
		MySpConstCol mspccol(mspcol);
		// MySpConstCol& mspccol2(mspcol); // NG
		// mspccol[0]->method(); // NG
		mspccol[0]->const_method();
	}
	{
		MyDerivedSp msp(new MyDerived);
		MyDerivedConstSp mcsp(msp);
		
		foo(msp);
		foo_cref(msp);
		foo_const_sp(msp);
		foo_const_sp_cref(msp);

		// foo(mcsp);  // NG
		// foo_cref(mcsp); // NG
		foo_const_sp(mcsp);
		foo_const_sp_cref(mcsp);

		MySpCol mspcol;
		mspcol.push_back(msp);
		// mspcol.push_back(mcsp); // NG
		// MyConstSpCol mcspcol(mspcol);
		MySpConstCol mspccol(mspcol);
		// MySpConstCol& mspccol2(mspcol); // NG
		// mspccol[0]->method(); // NG
		mspccol[0]->const_method();
	}

}

