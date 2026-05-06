#include "drevidencemovie.h"

#include "aoapplication.h"

DREvidenceMovie::DREvidenceMovie(AOApplication *ao_app, QGraphicsItem *parent)
    : DRMovie(parent)
    , ao_app(ao_app)
{
  set_scaling_mode(ScalingMode::HeightScaling);
}

DREvidenceMovie::~DREvidenceMovie()
{}

void DREvidenceMovie::play(QString p_file_name)
{
  QString path = ao_app->get_evidence_path(p_file_name);
  QString evidence_image_path = ao_app->find_asset_path({path});//, FS::Formats::StaticImages());
  if (evidence_image_path.isEmpty())
  {
    evidence_image_path = ao_app->find_asset_path({ao_app->get_evidence_path("empty.png")});
    if (evidence_image_path.isEmpty())
    {
      evidence_image_path = ":data/empty.png";
    }
  }
  set_file_name(evidence_image_path);
  start();
}