cs249a_p2
=========

shipping simulation

---------------------------
Design Notes
---------------------------

Our design ensures that all shipping simulation state is stored in entity objects, and 'processing' is done by the Engine (network topology) and Connection (path querying) classes. The Engine class provides factory methods (called by Rep-layer) for creating shipping entity objects. The Engine instance does not keep direct pointers to entity objects but instead a collection of LocationReactor and SegmentReactor notifiee objects. The individual reactors will respond to entity mutators and modify related entity objects if necessary (such as updating a location's segment list when the source attribute is set for a segment). Our design tracks network statistics using a Stats class that is a notifee of the Engine class; Stats is notified of relevant network events by 'trampolining' notifications from individual entity reactors up through the Engine class, finally reaching Stats. Path & explore queries are generated on demand by the Conn class. Location entity references and constraining attributes are provided to Conn methods by the Rep layer, and Conn objects can access the Engine instance to obtain Fleet values for computing cost, time, etc.

