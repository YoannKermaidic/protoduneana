////////////////////////////////////////////////////////////////////////
// Class:       CheckMCevents
// Plugin Type: analyzer (Yoann Kermaidic)
// File:        CheckMCevents_module.cc
//
// Generated at Thu Jun  8 16:51:19 2023 by Yoann Kermaidic using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

// ART
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// LArSoft
#include "larsim/MCCheater/BackTrackerService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"

// DUNE
#include "protoduneana/Utilities/ProtoDUNETruthUtils.h"

// ROOT
#include "TGraph.h"
#include "TCanvas.h"

using std::vector;
using std::string;


namespace pdvdana {
  class CheckMCevents;
}


class pdvdana::CheckMCevents : public art::EDAnalyzer {
public:
  explicit CheckMCevents(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  CheckMCevents(CheckMCevents const&) = delete;
  CheckMCevents(CheckMCevents&&) = delete;
  CheckMCevents& operator=(CheckMCevents const&) = delete;
  CheckMCevents& operator=(CheckMCevents&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;

  // Selected optional functions.
  void beginJob() override;
  void endJob() override;

private:

  // Declare member data here.
  int fEventNum;
  string fPFPModuleLabel = "pandora";
  string fHitModuleLabel = "hitpdune";
};


pdvdana::CheckMCevents::CheckMCevents(fhicl::ParameterSet const& p)
  : EDAnalyzer{p}// ,
  //fLogLevel(         		   p.get< int >("LogLevel"))
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void pdvdana::CheckMCevents::analyze(art::Event const& e)
{
   fEventNum = e.id().event();
   std::cout << "Event: " << fEventNum << std::endl;
   if (!e.isRealData()) std::cout << "  -> this is a MC event" << std::endl; 

   art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
//   auto const clockData = art::ServiceHandle<detinfo::DetectorClocksService>()->DataFor(e);

   protoana::ProtoDUNETruthUtils truthUtil;

   auto mcTruths      = e.getValidHandle<std::vector<simb::MCTruth>>("generator");
   auto true_particle = truthUtil.GetGeantGoodParticle((*mcTruths)[0],e);
   if (!true_particle) {
     std::cout << "No true beam particle" << std::endl;
   }
   else
     std::cout << "True PDG: " << true_particle->PdgCode() << std::endl;

/*
   // Hits
   std::vector<art::Ptr<recob::Hit> > hitlist;
   auto hitListHandle = event.getHandle< std::vector<recob::Hit> >(fHitModuleLabel);
   if (hitListHandle)
     art::fill_ptr_vector(hitlist, hitListHandle);

   // PFParticles
   std::vector < art::Ptr < recob::PFParticle > > pfpList;
   auto pfpListHandle = event.getHandle < std::vector < recob::PFParticle > >(fPFPModuleLabel);
   if (pfpListHandle) {
     art::fill_ptr_vector(pfpList, pfpListHandle);
   }
*/
   // ParticleList
   const sim::ParticleList & plist = pi_serv->ParticleList();

   std::cout << "Particle list size: " << plist.size() << std::endl;

   for( sim::ParticleList::const_iterator ipar = plist.begin(); ipar!=plist.end(); ++ipar){
     simb::MCParticle *particle = ipar->second;
     if (particle->Mother() == 0){ //primary particle
       std::cout << "Mother particle is " << particle->PdgCode() << std::endl;
     }
   }
  
/*
   double purity = truthUtil.GetPurity( clockData, *daughterPFP, e, fPFParticleTag);
   double completeness = truthUtil.GetCompleteness( clockData, *daughterPFP, e, fPFParticleTag, fHitTag );
 */

}

void pdvdana::CheckMCevents::beginJob()
{
  std::cout << "beginJob" << std::endl;
}

void pdvdana::CheckMCevents::endJob()
{
  std::cout << "endJob" << std::endl;
}

DEFINE_ART_MODULE(pdvdana::CheckMCevents)
