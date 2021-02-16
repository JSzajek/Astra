#pragma once

namespace Astra::Math
{
	#define PI	3.141592653589793238460f

	#define CMP_EPSILON 0.00001f
	#define CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)

	#ifdef ASTRA_DEBUG
		#define UNIT_EPSILON 0.00001f
	#else
		#define UNIT_EPSILON 0.001f // Lower tolerance
	#endif

	#undef MIN // override standard definition
	#undef MAX // override standard definition
	#undef SGN // override standard definition

	#ifndef MIN
	#define MIN(m_a, m_b) (((m_a) < (m_b)) ? (m_a) : (m_b))
	#endif

	#ifndef MAX
	#define MAX(m_a, m_b) (((m_a) > (m_b)) ? (m_a) : (m_b))
	#endif

	#ifndef SGN
	#define SGN(m_v) (((m_v) == 0) ? (0.0f) : (((m_v) < 0) ? (-1.0f) : (+1.0f)))
	#endif
}