/**
 @file    ee_phase_motion.cc
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jan 16, 2017
 @brief   Brief description
 */

#include <xpp/opt/ee_phase_motion.h>
#include <xpp/opt/polynomial_xd.h>

namespace xpp {
namespace opt {

EEPhaseMotion::EEPhaseMotion ()
{
}

EEPhaseMotion::~EEPhaseMotion ()
{
}

void
EEPhaseMotion::Init (double T, double h, const Vector3d& start,
                     const Vector3d& end)
{
  T_ = T;
  poly_z_.SetShape(6,h);
  SetContacts(start, end);
}

double
EEPhaseMotion::GetDuration () const
{
  return T_;
}

void
EEPhaseMotion::SetContacts (const Vector3d& start_pos,
                            const Vector3d& end_pos)
{
  StateLin3d start_state(start_pos);
  StateLin3d end_state(end_pos);
  poly_xy_.SetBoundary(T_, start_state.Get2D(), end_state.Get2D());
  poly_z_.SetBoundary(T_,start_state.Get1d(Z), end_state.Get1d(Z));
}

StateLin3d
EEPhaseMotion::GetState (double t_local) const
{
  StateLin1d z;
  poly_z_.GetPoint(t_local, z);

  StateLin2d xy;
  poly_xy_.GetPoint(t_local, xy);

  StateLin3d ee;
  ee.SetDimension(xy.Get1d(X), X);
  ee.SetDimension(xy.Get1d(Y), Y);
  ee.SetDimension(z, Z);

  return ee;
}

double
EEPhaseMotion::GetDerivativeOfPosWrtContactsXY (d2::Coords dim, double t_local,
                                                Polynomial::PointType p) const
{
  return poly_xy_.GetDim(dim).GetDerivativeOfPosWrtPos(t_local, p);
}

} /* namespace opt */
} /* namespace xpp */