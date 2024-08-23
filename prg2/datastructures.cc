/**
 * @brief The Datastructures class
 * STUDENTS: Modify the code below to implement the functionality of the class.
 * Also remove comments from the parameter names when you implement an operation
 * (Commenting out parameter name prevents compiler from warning about unused
 * parameters on operations you haven't yet implemented.)
 */

#include "datastructures.hh"
#include "customtypes.hh"

Datastructures::Datastructures()
{
  // Write any initialization you need here
}

Datastructures::~Datastructures()
{
  // Write any cleanup you need here
}

unsigned int Datastructures::get_bite_count()
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bite_count");
}

void Datastructures::clear_all()
{
  // Replace the line below with your implementation
  throw NotImplemented("clear_all");
}

std::vector<BiteID> Datastructures::all_bites()
{
  // Replace the line below with your implementation
  throw NotImplemented("all_bites");
}

bool Datastructures::add_bite(BiteID /*id*/, const Name & /*name*/, Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_bite");
}

Name Datastructures::get_bite_name(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bite_name");
}

Coord Datastructures::get_bite_coord(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bite_coord");
}

std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bites_alphabetically");
}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bites_distance_increasing");
}

BiteID Datastructures::find_bite_with_coord(Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("find_bite_with_coord");
}

bool Datastructures::add_contour(ContourID /*id*/, const Name & /*name*/, ContourHeight /*height*/,
                                 std::vector<Coord> /*coords*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_contour");
}

std::vector<ContourID> Datastructures::all_contours()
{
  // Replace the line below with your implementation
  throw NotImplemented("all_contours");
}

Name Datastructures::get_contour_name(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_contour_name");
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_contour_coords");
}

ContourHeight Datastructures::get_contour_height(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_contour_height");
}

bool Datastructures::add_subcontour_to_contour(ContourID /*id*/,
                                               ContourID /*parentid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_subcontour_to_contour");
}

bool Datastructures::add_bite_to_contour(BiteID /*biteid*/, ContourID /*contourid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_bite_to_contour");
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bite_in_contours");
}

std::vector<ContourID>
Datastructures::all_subcontours_of_contour(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("all_subcontours_of_contour");
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID /*id1*/,
                                                        ContourID /*id2*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_closest_common_ancestor_of_contours");
}

bool Datastructures::remove_bite(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("remove_bite");
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bites_closest_to");
}

/**
 * ============================
 * ============================
 * ========== PRG2 ============
 * ============================
 * ============================
 */

bool Datastructures::add_connection(ConnectionID /*connectionid*/, BiteID /*id1*/, BiteID /*id2*/,
                                    std::vector<Coord> /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_connection");
}

void Datastructures::clear_connections()
{
  // Replace the line below with your implementation
  throw NotImplemented("clear_connections");
}

std::vector<BiteID> Datastructures::get_next_bites_from(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_next_bites_from");
}

std::vector<ConnectionID> Datastructures::get_connections(BiteID id1, BiteID id2)
{
  (void)id1; //TODO: remove, a trick to get rid of the warning
  (void)id2; //TODO: remove, a trick to get rid of the warning
  // Replace the line below with your implementation
  throw NotImplemented("get_connections");
}

std::vector<Coord> Datastructures::get_connection_coords(BiteID /*biteid*/,
                                                         ConnectionID /*connectionid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_connection_coords");
}

std::vector<std::pair<Coord, Distance>> Datastructures::path_any(BiteID /*fromid*/, BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_any");
}
// Non-compulsory operations
std::vector<std::pair<Coord, Distance>> Datastructures::path_shortest(BiteID /*fromid*/,
                                                                      BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_shortest");
}
std::vector<std::pair<Coord, Distance>> Datastructures::path_least_bites(BiteID /*fromid*/,
                                                                         BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_least_bites");
}
std::vector<std::pair<Coord, Distance>> Datastructures::path_least_uphill(BiteID /*fromid*/,
                                                                          BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_least_uphill");
}
