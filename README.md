user: dcocuzzo
user: oreccp

============
Design Notes
============

Our design ensures that all shipping simulation state is stored in entity objects 
(Engine Layer), and processing is done by the Engine class. The Engine class provides
factory methods (called by Rep-layer) for creating and deleting shipping entity
objects. The Engine instance does not keep direct pointers to entity objects but
instead a collection of LocationReactor and SegmentReactor notifiee objects
("very smart pointers"); the individual reactors respond to entity mutators
and modify related entity objects if necessary (such as updating a location's
segment list when the source attribute is set for a segment). This creates a
centralized module for processing, and entity objects are essentially
strongly-typed containers for consistent state.

Our design tracks network statistics using a Stats class that is a notifee of the
Engine class; Stats is notified of relevant network events by 'trampolining'
notifications from individual entity reactors up through the Engine class, which
processes notifications and propagates relevant information to Stats through the
usual notification model.

Connect and explore queries are implemented as Engine methods, since per-query
results do not persist. We have developed Path and ConstrainedPath classes to support
the connect and explore queries, respectively. These classes encapsulate the network
consistency, entity type checking, and constraint checking logic by providing a
robust segmentAdd method. In addition, we offload the work of computing a segment's
cost or time contribution from the Path class to the Engine class, which can
directly access the relevant Fleet attributes. These two design choices allow the
graph search algorithms implemented by the engine to support connect/explore to
be quite simple.

We have chosen to keep specific entity-type classes (i.e. BoatTerminal) within our
implementation (Engine.h) for flexibility and extensibility, although we currently
only instantiate generic Location/Segment entities that contain an enumerated value
indicating the Location/Segment type. 