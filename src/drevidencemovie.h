#pragma once

#include "drmovie.h"

class AOApplication;

class DREvidenceMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DREvidenceMovie(AOApplication *ao_app, QGraphicsItem *parent = nullptr);
  ~DREvidenceMovie();

  void play(QString p_file_name);

private:
  AOApplication *ao_app;
};
