#pragma once

#include <cmath>
#include <type_traits>

namespace Draggoon {

	/// <summary>Holds dimensions or coordinates about a 2D position.</summary>
	template<class T=int>
	class Vector2D {
	public:
		Vector2D(const T& t_x = 0, const T& t_y = 0);
		T getX() const;
		T getY() const;
		void setX(const T& t_x);
		void setY(const T& t_y);
		
		T getLength() const;
		T getArea() const;

		T distanceTo(const Vector2D<T> t_v);

		virtual bool contains(const Vector2D<T> t_v) const;
		virtual bool isContainedIn(const Vector2D<T> t_v) const;
		virtual bool operator==(const Vector2D<T>& t_v) const;
		virtual bool operator!=(const Vector2D<T>& t_v) const;
	protected:
		T m_x;	/// <summary>Horizontal coordinate (left to right).</summary>
		T m_y;	/// <summary>Vertical coordinate (top to bottom).</summary>
	};

	/// <summary>Holds coordinates about a 3D position.</summary>
	template<class T=int>
	class Vector3D : public Vector2D<T> {
	public:
		Vector3D(const T& t_x=0, const T& t_y=0, const T& t_z=0);
		T getZ() const;
		void setZ(const T& t_z);

		T getArea() const = delete;
		T getVolume() const;
		Vector2D<T> projectionXY() const;

		virtual bool contains(const Vector3D<T> t_v) const;
		virtual bool isContainedIn(const Vector3D<T> t_v) const;
		virtual bool operator==(const Vector3D<T>& t_v) const;
		virtual bool operator!=(const Vector3D<T>& t_v) const;

	protected:
		T m_z;	/// <summary>Deepness coordinate (front to back).</summary>


	};

}
