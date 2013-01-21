/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2008-2009 Marcus D. Hanwell
  Copyright 2010-2012 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#ifndef AVOGADRO_QTGUI_CUBE_H
#define AVOGADRO_QTGUI_CUBE_H

#include "avogadroqtguiexport.h"

#include <avogadro/core/vector.h>

#include <vector>
#include <QtCore/QString>

// Forward declarations
class QReadWriteLock;

namespace Avogadro {
namespace QtGui {

class Molecule;

/**
 * @class Cube cube.h <avogadro/qtgui/cube.h>
 * @brief Provide a data structure for regularly spaced 3D grids.
 * @author Marcus D. Hanwell
 */

class AVOGADROQTGUI_EXPORT Cube
{
public:
  Cube();
  ~Cube();

  /**
   * \enum Different Cube types relating to the data
   */
  enum Type{
    VdW,
    ESP,
    ElectronDensity,
    MO,
    FromFile,
    None
  };

  /**
   * @return The minimum point in the cube.
   */
  Vector3 min() const { return m_min; }

  /**
   * @return The maximum point in the cube.
   */
  Vector3 max() const { return m_max; }

  /**
   * @return The spacing of the grid.
   */
  Vector3 spacing() const { return m_spacing; }

  /**
   * @return The x, y and z dimensions of the cube.
   */
  Vector3i dimensions() const { return m_points; }

  /**
   * Set the limits of the cube.
   * @param min The minimum point in the cube.
   * @param max The maximum point in the cube.
   * @param points The number of (integer) points in the cube.
   */
  bool setLimits(const Vector3 &min, const Vector3 &max,
                 const Vector3i &points);

  /**
   * Set the limits of the cube.
   * @param min The minimum point in the cube.
   * @param max The maximum point in the cube.
   * @param spacing The interval between points in the cube.
   */
  bool setLimits(const Vector3 &min, const Vector3 &max,
                 double spacing);

  /**
   * Set the limits of the cube.
   * @param min The minimum point in the cube.
   * @param dim The integer dimensions of the cube in x, y and z.
   * @param spacing The interval between points in the cube.
   */
  bool setLimits(const Vector3 &min, const Vector3i &dim,
                 double spacing);

  /**
   * Set the limits of the cube - copy the limits of an existing Cube.
   * @param cube Existing Cube to copy the limits from.
   */
  bool setLimits(const Cube &cube);

  /**
   * Set the limits of the cube.
   * @param mol Molecule to take limits from
   * @param spacing The spacing of the regular grid
   * @param padding Padding around the molecule
   */
  bool setLimits(const Molecule &mol, double spacing, double padding);

  /**
   * @return Vector containing all the data in a one-dimensional array.
   */
  std::vector<double> * data();

  /**
   * Set the values in the cube to those passed in the vector.
   */
  bool setData(const std::vector<double> &values);

  /**
   * Adds the values in the cube to those passed in the vector.
   */
  bool addData(const std::vector<double> &values);

  /**
   * @return Index of the point closest to the position supplied.
   * @param pos Position to get closest index for.
   */
  unsigned int closestIndex(const Vector3 &pos) const;

  /**
   * @param pos Position to get closest index for.
   * @return The i, j, k index closest to the position supplied.
   */
  Vector3i indexVector(const Vector3 &pos) const;

  /**
   * @param index Index to be translated to a position.
   * @return Position of the given index.
   */
  Vector3 position(unsigned int index) const;

  /**
   * This function is very quick as it just returns the value at the point.
   * @return Cube value at the integer point i, j, k.
   */
  double value(int i, int j, int k) const;

  /**
   * This function is very quick as it just returns the value at the point.
   * @return Cube value at the integer point pos.
   */
  double value(const Vector3i &pos) const;

  /**
   * This function uses trilinear interpolation to find the value at points
   * between those specified in the cube.
   * @return Cube value at the specified position.
   * @warning This function is quite computationally expensive and should be
   * avoided where possible.
   */
  float valuef(const Vector3f &pos) const;

  /**
   * This function uses trilinear interpolation to find the value at points
   * between those specified in the cube.
   * @return Cube value at the specified position.
   * @warning This function is quite computationally expensive and should be
   * avoided where possible.
   */
  double value(const Vector3 &pos) const;

  /**
   * Sets the value at the specified point in the cube.
   * @param i x compenent of the position.
   * @param j y compenent of the position.
   * @param k z compenent of the position.
   * @param value Value at the specified position.
   */
  bool setValue(int i, int j, int k, double value);

  /**
   * Sets the value at the specified index in the cube.
   * @param i 1-dimenional index of the point to set in the cube.
   */
  bool setValue(unsigned int i, double value);

  /**
   * @return The minimum  value at any point in the Cube.
   */
  double minValue() const { return m_minValue; }

  /**
   * @return The maximum  value at any point in the Cube.
   */
  double maxValue() const { return m_maxValue; }

  void setName(const QString &name_) { m_name = name_; }
  QString name() const { return m_name; }

  void setName(const char *name);
  const char * name_c_str() const;

  void setCubeType(Type type) { m_cubeType = type; }
  Type cubeType() const { return m_cubeType; }

  /**
   * Provides locking.
   */
  QReadWriteLock * lock() const;

protected:
  std::vector<double> m_data;
  Vector3 m_min, m_max, m_spacing;
  Vector3i m_points;
  double m_minValue, m_maxValue;
  QString m_name;
  Type    m_cubeType;
  QReadWriteLock *m_lock;
};

inline bool Cube::setValue(unsigned int i, double value_)
{
  if (i < m_data.size()) {
    m_data[i] = value_;
    if (value_ > m_maxValue)
      m_maxValue = value_;
    if (value_ < m_minValue)
      m_minValue = value_;
    return true;
  }
  else
    return false;
}

} // End QtGui namespace
} // End Avogadro namespace

#endif // AVOGADRO_QTGUI_CUBE_H