#ifndef LIGHT_PROSE_H
#define LIGHT_PROSE_H

#ifdef NDEBUG
#define SENTENCE static inline void
#else
#define SENTENCE void
#endif

#ifdef NDEBUG
#define WORD static inline
#else
#define WORD
#endif

#endif
