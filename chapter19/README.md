some concepts in C++
1.polymorphism:
    The reason that both
    styles of programming are called polymorphism is that each style relies on the pro-
    grammer to present many versions of a concept by a single interface. Polymorphism
    is Greek for “many shapes,” referring to the many different types you can manip-
    ulate through a common interface.
2.To sum up:
• Generic programming: supported by templates, relying on compiletime resolution
• Object-oriented programming: supported by class hierarchies and virtual functions, relying on run-time resolution
3.Concepts.
  We call a set of requirements on a template argument a concept. A template argument must meet the requirements, the concepts, of the template to which it is applied
4.Just remember that “D is a B” does not imply “C<D> is a C<B>” for an arbitrary template C.
5.RAII
 it applies to all kinds of resources, acquire a resource in the constructor for some object that manages
it, and release it again in the matching destructor. Examples of resources that are
usually best dealt with in this way include database locks, sockets, and I/O buffers. 

    sv.read_val(iss);报错，error: no match for ‘operator>>’ (operand types are ‘std::istream’ {aka ‘std::basic_istream<char>’} and ‘std::vector<int>’