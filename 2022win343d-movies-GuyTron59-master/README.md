Addressing clang-tidy errors:
The anonymous factories listed below:
anonymousClassicMovieFactory
anonymousComedyMovieFactory
anonymousDramaMovieFactory

Are listed as errors, but our class design follows self-registering factories
that require global abstract factories. This cannot be addressed and is one of the flaws
of using a design like this.