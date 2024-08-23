// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include "customtypes.hh"
#include <utility>
#include <vector>

// Add your own STL includes below this comment

template <typename Type>
Type random_in_range(Type start, Type end);

// This is the class you are supposed to implement

class Datastructures
{
public:
  Datastructures();
  ~Datastructures();

  // Estimate of performance:
  // Short rationale for estimate:
  unsigned int get_bite_count();

  // Estimate of performance:
  // Short rationale for estimate:
  void clear_all();

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<BiteID> all_bites();

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_bite(BiteID /*id*/, const Name & /*name*/, Coord /*xy*/);

  // Estimate of performance:
  // Short rationale for estimate:
  Name get_bite_name(BiteID /*id*/);

  // Estimate of performance:
  // Short rationale for estimate:
  Coord get_bite_coord(BiteID /*id*/);

  // We recommend you implement the operations below only after implementing the
  // ones above

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<BiteID> get_bites_alphabetically();

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<BiteID> get_bites_distance_increasing();

  // Estimate of performance:
  // Short rationale for estimate:
  BiteID find_bite_with_coord(Coord /*xy*/);


  // Estimate of performance:
  // Short rationale for estimate:
  bool add_contour(ContourID /*id*/, const Name & /*name*/, ContourHeight /*height*/,
                   std::vector<Coord> /*coords*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<ContourID> all_contours();

  // Estimate of performance:
  // Short rationale for estimate:
  Name get_contour_name(ContourID /*id*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<Coord> get_contour_coords(ContourID /*id*/);

  // Estimate of performance:
  // Short rationale for estimate:
  ContourHeight get_contour_height(ContourID /*id*/);

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_subcontour_to_contour(ContourID /*id*/, ContourID /*parentid*/);

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_bite_to_contour(BiteID biteid, ContourID /*parentid*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<ContourID> get_bite_in_contours(BiteID /*id*/);

  // Non-compulsory operations

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<ContourID> all_subcontours_of_contour(ContourID /*id*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<BiteID> get_bites_closest_to(Coord /*xy*/);

  // Estimate of performance:
  // Short rationale for estimate:
  bool remove_bite(BiteID /*id*/);

  // Estimate of performance:
  // Short rationale for estimate:
  ContourID get_closest_common_ancestor_of_contours(ContourID /*id1*/,
                                                    ContourID /*id2*/);




 /**
   * ============================
   * ============================
   * ========== PRG2 ============
   * ============================
   * ============================
   */

  // Estimate of performance: 
  // Short rationale for estimate: 
  bool add_connection(ConnectionID /*connectionid*/, BiteID /*id1*/, BiteID /*id2*/,
                      std::vector<Coord> /*xy*/);

  // Estimate of performance: 
  // Short rationale for estimate: 
  void clear_connections();

  // Estimate of performance: 
  // Short rationale for estimate: 
  std::vector<BiteID> get_next_bites_from(BiteID /*id*/);

  // Estimate of performance: 
  // Short rationale for estimate: 
  std::vector<ConnectionID> get_connections(BiteID id1 = NO_BITE, BiteID id2 = NO_BITE);

  // Estimate of performance: 
  // Short rationale for estimate: 
  std::vector<Coord> get_connection_coords(BiteID /*biteid*/,
                                           ConnectionID /*connectionid*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_any(BiteID /*fromid*/, BiteID /*toid*/);

  // Non-compulsory operations

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_shortest(BiteID /*fromid*/,
                                                        BiteID /*toid*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_least_bites(BiteID /*fromid*/,
                                                           BiteID /*toid*/);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_least_uphill(BiteID /*fromid*/,
                                                            BiteID /*toid*/);



private:
  // Add stuff needed for your class implementation here
};

#endif // DATASTRUCTURES_HH
