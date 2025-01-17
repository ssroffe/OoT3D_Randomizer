#include "hints.hpp"

#include "custom_messages.hpp"
#include "dungeon.hpp"
#include "item_location.hpp"
#include "item_pool.hpp"
#include "location_access.hpp"
#include "logic.hpp"
#include "random.hpp"
#include "spoiler_log.hpp"
#include "fill.hpp"
#include "hint_list.hpp"
#include "trial.hpp"

using namespace CustomMessages;
using namespace Logic;
using namespace Settings;
using namespace Trial;

constexpr std::array<HintSetting, 4> hintSettingTable{{
  // Useless hints
  {
    .dungeonsWothLimit = 2,
    .dungeonsBarrenLimit = 1,
    .namedItemsRequired = false,
    .distTable = {{
      {.type = HintType::Trial,     .order =  1, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Always,    .order =  2, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Woth,      .order =  3, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Barren,    .order =  4, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Entrance,  .order =  5, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Sometimes, .order =  6, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Random,    .order =  7, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Item,      .order =  8, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Song,      .order =  9, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Overworld, .order = 10, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Dungeon,   .order = 11, .weight =  0, .fixed = 0, .copies = 0},
      {.type = HintType::Junk,      .order = 12, .weight = 99, .fixed = 0, .copies = 0},
      {.type = HintType::NamedItem, .order = 13, .weight =  0, .fixed = 0, .copies = 0},
    }},
  },

  // Balanced hints
  {
    .dungeonsWothLimit = 2,
    .dungeonsBarrenLimit = 1,
    .namedItemsRequired = true,
    .distTable = {{
      {.type = HintType::Trial,     .order =  1, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Always,    .order =  2, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Woth,      .order =  3, .weight =  7, .fixed = 0, .copies = 1},
      {.type = HintType::Barren,    .order =  4, .weight =  4, .fixed = 0, .copies = 1},
      {.type = HintType::Entrance,  .order =  5, .weight =  6, .fixed = 0, .copies = 1},
      {.type = HintType::Sometimes, .order =  6, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Random,    .order =  7, .weight = 12, .fixed = 0, .copies = 1},
      {.type = HintType::Item,      .order =  8, .weight = 10, .fixed = 0, .copies = 1},
      {.type = HintType::Song,      .order =  9, .weight =  2, .fixed = 0, .copies = 1},
      {.type = HintType::Overworld, .order = 10, .weight =  4, .fixed = 0, .copies = 1},
      {.type = HintType::Dungeon,   .order = 11, .weight =  3, .fixed = 0, .copies = 1},
      {.type = HintType::Junk,      .order = 12, .weight =  6, .fixed = 0, .copies = 1},
      {.type = HintType::NamedItem, .order = 13, .weight =  0, .fixed = 0, .copies = 1},
    }},
  },

  // Strong hints
  {
    .dungeonsWothLimit = 2,
    .dungeonsBarrenLimit = 1,
    .namedItemsRequired = true,
    .distTable = {{
      {.type = HintType::Trial,     .order =  1, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Always,    .order =  2, .weight =  0, .fixed = 0, .copies = 2},
      {.type = HintType::Woth,      .order =  3, .weight = 12, .fixed = 0, .copies = 2},
      {.type = HintType::Barren,    .order =  4, .weight = 12, .fixed = 0, .copies = 1},
      {.type = HintType::Entrance,  .order =  5, .weight =  4, .fixed = 0, .copies = 1},
      {.type = HintType::Sometimes, .order =  6, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Random,    .order =  7, .weight =  8, .fixed = 0, .copies = 1},
      {.type = HintType::Item,      .order =  8, .weight =  8, .fixed = 0, .copies = 1},
      {.type = HintType::Song,      .order =  9, .weight =  4, .fixed = 0, .copies = 1},
      {.type = HintType::Overworld, .order = 10, .weight =  6, .fixed = 0, .copies = 1},
      {.type = HintType::Dungeon,   .order = 11, .weight =  6, .fixed = 0, .copies = 1},
      {.type = HintType::Junk,      .order = 12, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::NamedItem, .order = 13, .weight =  0, .fixed = 0, .copies = 1},
    }},
  },

  // Very strong hints
  {
    .dungeonsWothLimit = 40,
    .dungeonsBarrenLimit = 40,
    .namedItemsRequired = true,
    .distTable = {{
      {.type = HintType::Trial,     .order =  1, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Always,    .order =  2, .weight =  0, .fixed = 0, .copies = 2},
      {.type = HintType::Woth,      .order =  3, .weight = 15, .fixed = 0, .copies = 2},
      {.type = HintType::Barren,    .order =  4, .weight = 15, .fixed = 0, .copies = 1},
      {.type = HintType::Entrance,  .order =  5, .weight = 10, .fixed = 0, .copies = 1},
      {.type = HintType::Sometimes, .order =  6, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Random,    .order =  7, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::Item,      .order =  8, .weight =  5, .fixed = 0, .copies = 1},
      {.type = HintType::Song,      .order =  9, .weight =  2, .fixed = 0, .copies = 1},
      {.type = HintType::Overworld, .order = 10, .weight =  7, .fixed = 0, .copies = 1},
      {.type = HintType::Dungeon,   .order = 11, .weight =  7, .fixed = 0, .copies = 1},
      {.type = HintType::Junk,      .order = 12, .weight =  0, .fixed = 0, .copies = 1},
      {.type = HintType::NamedItem, .order = 13, .weight =  0, .fixed = 0, .copies = 1},
    }},
  },
}};

static Exit* GetHintRegion(Exit* exit) {

  std::vector<Exit*> alreadyChecked = {};
  std::vector<Exit*> spotQueue = {exit};

  while (!spotQueue.empty()) {
    Exit* region = spotQueue.back();
    alreadyChecked.push_back(region);
    spotQueue.pop_back();

    if (region->hintText != &Hints::NoHintText) {
      return region;
    }

    //add unchecked exits to spot queue
    bool checked = false;
    for (auto exitPair : region->exits) {
      Exit* e = exitPair.GetExit();

      for (Exit* checkedExit : alreadyChecked) {
        if (e == checkedExit) {
          checked = true;
          break;
        }
      }

      if (!checked) {
        spotQueue.insert(spotQueue.begin(), e);
      }
    }
  }

  return &Exits::NoExit;
}

static std::vector<ItemLocation*> GetAccessibleGossipStones(ItemLocation* hintedLocation = &Ganon) {
  //temporarily remove the hinted location's item, and then perform a
  //reachability search for gossip stone locations.
  Item originalItem = hintedLocation->GetPlacedItem();
  hintedLocation->SetPlacedItem(NoItem);

  LogicReset();
  auto accessibleGossipStones = GetAccessibleLocations(gossipStoneLocations);
  //Give the item back to the location
  hintedLocation->SetPlacedItem(originalItem);

  return accessibleGossipStones;
}

static void AddHint(Text hint, ItemLocation* gossipStone, const std::vector<u8>& colors = {}) {
  //save hints as dummy items to gossip stone locations for writing to the spoiler log
  gossipStone->SetPlacedItem(Item{hint.GetEnglish(), ITEMTYPE_EVENT, GI_RUPEE_BLUE_LOSE, false, &noVariable, &Hints::NoHintText});

  //create the in game message
  u32 messageId = 0x400 + gossipStone->GetFlag();
  CreateMessageFromTextObject(messageId, 0, 2, 3, AddColorsAndFormat(hint, colors));
}

static void CreateLocationHint(const std::vector<ItemLocation*>& possibleHintLocations) {
  //return if there aren't any hintable locations or gossip stones available
  if (possibleHintLocations.empty()) {
    PlacementLog_Msg("\tNO LOCATIONS TO HINT\n\n");
    return;
  }

  ItemLocation* hintedLocation = RandomElement(possibleHintLocations);
  const std::vector<ItemLocation*> accessibleGossipStones = GetAccessibleGossipStones(hintedLocation);

  PlacementLog_Msg("\tLocation: ");
  PlacementLog_Msg(hintedLocation->GetName());
  PlacementLog_Msg("\n");

  PlacementLog_Msg("\tItem: ");
  PlacementLog_Msg(hintedLocation->GetPlacedItemName());
  PlacementLog_Msg("\n");

  if (accessibleGossipStones.empty()) {
    PlacementLog_Msg("\tNO GOSSIP STONES TO PLACE HINT\n\n");
    return;
  }

  ItemLocation* gossipStone = RandomElement(accessibleGossipStones);
  hintedLocation->SetAsHinted();

  //make hint text
  Text locationHintText = hintedLocation->GetHintText().GetText();
  Text itemHintText = hintedLocation->GetPlacedItem().GetHintText().GetText();
  Text prefix = Hints::Prefix.GetText();

  Text finalHint = prefix + locationHintText + " #"+itemHintText+"#.";
  PlacementLog_Msg("\tMessage: ");
  PlacementLog_Msg(finalHint.english);
  PlacementLog_Msg("\n\n");
  AddHint(finalHint, gossipStone, {QM_GREEN, QM_RED});
}

static void CreateWothHint(u8* remainingDungeonWothHints) {
  //get locations that are in the current playthrough
  std::vector<ItemLocation*> possibleHintLocations = {};
  //iterate through playthrough locations by sphere
  for (std::vector<ItemLocation*> sphere : playthroughLocations) {
    std::vector<ItemLocation*> sphereHintLocations = FilterFromPool(sphere, [remainingDungeonWothHints](ItemLocation* loc){
      return loc->IsHintable()  && //only filter hintable locations
            !loc->IsHintedAt()  && //only filter locations that haven't been hinted at
            (loc->IsOverworld() || (loc->IsDungeon() && (*remainingDungeonWothHints) > 0)); //make sure we haven't surpassed the woth dungeon limit
    });
    AddElementsToPool(possibleHintLocations, sphereHintLocations);
  }

  //If no more locations can be hinted at for woth, then just try to get another hint
  if (possibleHintLocations.empty()) {
    PlacementLog_Msg("\tNO LOCATIONS TO HINT\n\n");
    return;
  }
  ItemLocation* hintedLocation = RandomElement(possibleHintLocations);

  PlacementLog_Msg("\tLocation: ");
  PlacementLog_Msg(hintedLocation->GetName());
  PlacementLog_Msg("\n");

  PlacementLog_Msg("\tItem: ");
  PlacementLog_Msg(hintedLocation->GetPlacedItemName());
  PlacementLog_Msg("\n");

  //get an accessible gossip stone
  const std::vector<ItemLocation*> gossipStoneLocations = GetAccessibleGossipStones(hintedLocation);

  if (gossipStoneLocations.empty()) {
    PlacementLog_Msg("\tNO GOSSIP STONES TO PLACE HINT\n\n");
    return;
  }
  hintedLocation->SetAsHinted();
  ItemLocation* gossipStone = RandomElement(gossipStoneLocations);

  //form hint text
  Text locationText;
  if (hintedLocation->IsDungeon()) {
    *remainingDungeonWothHints -= 1;
    locationText = hintedLocation->GetParentRegion()->hintText->GetText();
  } else {
    locationText = GetHintRegion(hintedLocation->GetParentRegion())->hintText->GetText();
  }
  Text finalWothHint = Hints::Prefix.GetText()+"#"+locationText+"#"+Hints::WayOfTheHero.GetText();
  PlacementLog_Msg("\tMessage: ");
  PlacementLog_Msg(finalWothHint.english);
  PlacementLog_Msg("\n\n");
  AddHint(finalWothHint, gossipStone, {QM_LBLUE});
}

static void CreateBarrenHint(u8* remainingDungeonBarrenHints, std::vector<ItemLocation*>& barrenLocations) {
  //remove dungeon locations if necessary
  if (*remainingDungeonBarrenHints < 1) {
    barrenLocations = FilterFromPool(barrenLocations, [](ItemLocation* loc){return !loc->IsDungeon();});
  }

  if (barrenLocations.empty()) {
    return;
  }

  ItemLocation* hintedLocation = RandomElement(barrenLocations, true);

  PlacementLog_Msg("\tLocation: ");
  PlacementLog_Msg(hintedLocation->GetName());
  PlacementLog_Msg("\n");

  PlacementLog_Msg("\tItem: ");
  PlacementLog_Msg(hintedLocation->GetPlacedItemName());
  PlacementLog_Msg("\n");

  //get an accessible gossip stone
  const std::vector<ItemLocation*> gossipStoneLocations = GetAccessibleGossipStones(hintedLocation);
  if (gossipStoneLocations.empty()) {
    PlacementLog_Msg("\tNO GOSSIP STONES TO PLACE HINT\n\n");
    return;
  }
  hintedLocation->SetAsHinted();
  ItemLocation* gossipStone = RandomElement(gossipStoneLocations);

  //form hint text
  Text locationText;
  if (hintedLocation->IsDungeon()) {
    *remainingDungeonBarrenHints -= 1;
    locationText = hintedLocation->GetParentRegion()->hintText->GetText();
  } else {
    locationText = GetHintRegion(hintedLocation->GetParentRegion())->hintText->GetText();
  }
  Text finalBarrenHint = Hints::Prefix.GetText()+Hints::Plundering.GetText()+"#"+locationText+"#"+Hints::Foolish.GetText();
  PlacementLog_Msg("\tMessage: ");
  PlacementLog_Msg(finalBarrenHint.english);
  PlacementLog_Msg("\n\n");
  AddHint(finalBarrenHint, gossipStone, {QM_PINK});

  //get rid of all other locations in this same barren region
  barrenLocations = FilterFromPool(barrenLocations, [hintedLocation](ItemLocation* loc){
    return GetHintRegion(loc->GetParentRegion())->hintText != GetHintRegion(hintedLocation->GetParentRegion())->hintText;
  });

}

static void CreateRandomLocationHint(bool goodItem = false) {
  const std::vector<ItemLocation*> possibleHintLocations = FilterFromPool(allLocations, [goodItem](const ItemLocation* loc) {
    return loc->IsHintable() && !loc->IsHintedAt() && (!goodItem || loc->GetPlacedItem().IsMajorItem());
  });
  //If no more locations can be hinted at, then just try to get another hint
  if (possibleHintLocations.empty()) {
    PlacementLog_Msg("\tNO LOCATIONS TO HINT\n\n");
    return;
  }
  ItemLocation* hintedLocation = RandomElement(possibleHintLocations);

  PlacementLog_Msg("\tLocation: ");
  PlacementLog_Msg(hintedLocation->GetName());
  PlacementLog_Msg("\n");

  PlacementLog_Msg("\tItem: ");
  PlacementLog_Msg(hintedLocation->GetPlacedItemName());
  PlacementLog_Msg("\n");

  //get an acessible gossip stone
  const std::vector<ItemLocation*> gossipStoneLocations = GetAccessibleGossipStones(hintedLocation);
  if (gossipStoneLocations.empty()) {
    PlacementLog_Msg("\tNO GOSSIP STONES TO PLACE HINT\n\n");
    return;
  }
  hintedLocation->SetAsHinted();
  ItemLocation* gossipStone = RandomElement(gossipStoneLocations);

  //form hint text
  Text itemText = hintedLocation->GetPlacedItem().GetHintText().GetText();
  if (hintedLocation->IsDungeon()) {
    Text locationText = hintedLocation->GetParentRegion()->hintText->GetText();
    Text finalHint = Hints::Prefix.GetText()+"#"+locationText+"# hoards #"+itemText+"#.";
    PlacementLog_Msg("\tMessage: ");
    PlacementLog_Msg(finalHint.english);
    PlacementLog_Msg("\n\n");
    AddHint(finalHint, gossipStone, {QM_GREEN, QM_RED});
  } else {
    Text locationText = GetHintRegion(hintedLocation->GetParentRegion())->hintText->GetText();
    Text finalHint = Hints::Prefix.GetText()+"#"+itemText+"# can be found at #"+locationText+"#.";
    PlacementLog_Msg("\tMessage: ");
    PlacementLog_Msg(finalHint.english);
    PlacementLog_Msg("\n\n");
    AddHint(finalHint, gossipStone, {QM_RED, QM_GREEN});
  }
}

static void CreateGoodItemHint() {
  CreateRandomLocationHint(true);
}

static void CreateJunkHint() {
  //duplicate junk hints are possible for now
  const HintText* junkHint = RandomElement(Hints::junkHints);
  LogicReset();
  const std::vector<ItemLocation*> gossipStones = GetAccessibleLocations(gossipStoneLocations);
  if (gossipStones.empty()) {
    PlacementLog_Msg("\tNO GOSSIP STONES TO PLACE HINT\n\n");
    return;
  }
  ItemLocation* gossipStone = RandomElement(gossipStones);
  Text hintText = junkHint->GetText();

  PlacementLog_Msg("\tMessage: ");
  PlacementLog_Msg(hintText.english);
  PlacementLog_Msg("\n\n");

  AddHint(hintText, gossipStone);
}

static std::vector<ItemLocation*> CalculateBarrenRegions() {
  std::vector<ItemLocation*> barrenLocations = {};
  std::vector<ItemLocation*> potentiallyUsefulLocations = {};

  for (ItemLocation* location : allLocations) {
    if (location->GetPlacedItem().IsMajorItem()) {
      AddElementsToPool(potentiallyUsefulLocations, std::vector{location});
    } else {
      if (location != &LinksPocket) { //Nobody cares to know if Link's Pocket is barren
        AddElementsToPool(barrenLocations, std::vector{location});
      }
    }
  }

  //leave only locations at barren regions in the list
  auto finalBarrenLocations = FilterFromPool(barrenLocations, [&potentiallyUsefulLocations](ItemLocation* loc){
    for (ItemLocation* usefulLoc : potentiallyUsefulLocations) {
      const HintText* barren = GetHintRegion(loc->GetParentRegion())->hintText;
      const HintText* useful = GetHintRegion(usefulLoc->GetParentRegion())->hintText;
      if (barren == useful) {
        return false;
      }
    }
    return true;
  });

  return finalBarrenLocations;
}

static void CreateTrialHints() {
    //six trials
  if (RandomGanonsTrials && GanonsTrialsCount.Is(6)) {

    //get a random gossip stone
    auto gossipStones = GetAccessibleGossipStones();
    auto gossipStone = RandomElement(gossipStones, false);

    //make hint
    auto hintText = Hints::Prefix.GetText() + Hints::SixTrials.GetText();
    AddHint(hintText, gossipStone, {QM_PINK});

    //zero trials
  } else if (RandomGanonsTrials && GanonsTrialsCount.Is(0)) {

    //get a random gossip stone
    auto gossipStones = GetAccessibleGossipStones();
    auto gossipStone = RandomElement(gossipStones, false);

    //make hint
    auto hintText = Hints::Prefix.GetText() + Hints::ZeroTrials.GetText();
    AddHint(hintText, gossipStone, {QM_YELLOW});

    //4 or 5 required trials
  } else if (GanonsTrialsCount.Is(5) || GanonsTrialsCount.Is(4)) {

    //get skipped trials
    std::vector<TrialInfo*> trials = {};
    trials.assign(trialList.begin(), trialList.end());
    auto skippedTrials = FilterFromPool(trials, [](TrialInfo* trial){return trial->IsSkipped();});

    //create a hint for each skipped trial
    for (auto& trial : skippedTrials) {
      //get a random gossip stone
      auto gossipStones = GetAccessibleGossipStones();
      auto gossipStone = RandomElement(gossipStones, false);

      //make hint
      auto hintText = Hints::Prefix.GetText()+"#"+trial->GetName()+"#"+Hints::FourToFiveTrials.GetText();
      AddHint(hintText, gossipStone, {QM_YELLOW});
    }
    //1 to 3 trials
  } else if (GanonsTrialsCount.Value<u8>() >= 1 && GanonsTrialsCount.Value<u8>() <= 3) {
    //get requried trials
    std::vector<TrialInfo*> trials = {};
    trials.assign(trialList.begin(), trialList.end());
    auto requiredTrials = FilterFromPool(trials, [](TrialInfo* trial){return trial->IsRequired();});

    //create a hint for each required trial
    for (auto& trial : requiredTrials) {
      //get a random gossip stone
      auto gossipStones = GetAccessibleGossipStones();
      auto gossipStone = RandomElement(gossipStones, false);

      //make hint
      auto hintText = Hints::Prefix.GetText()+"#"+trial->GetName()+"#"+Hints::OneToThreeTrials.GetText();
      AddHint(hintText, gossipStone, {QM_PINK});
    }
  }
}

static void CreateGanonText() {

  //funny ganon line
  auto ganonText = RandomElement(Hints::ganonLines)->GetText();
  CreateMessageFromTextObject(0x70CB, 0, 2, 3, AddColorsAndFormat(ganonText));

  //Get the location of the light arrows
  auto lightArrowLocation = FilterFromPool(allLocations, [](ItemLocation* loc){return loc->GetPlacedItem() == I_LightArrows;});

  Text text;
  //If there is no light arrow location, it was in the player's inventory at the start
  if (lightArrowLocation.empty()) {
    text = Hints::LightArrowLocation.GetText()+Hints::YourPocket.GetText();
  } else {
    text = Hints::LightArrowLocation.GetText()+GetHintRegion(lightArrowLocation[0]->GetParentRegion())->hintText->GetText();
  }
  text = text + "!";

  CreateMessageFromTextObject(0x70CC, 0, 2, 3, AddColorsAndFormat(text));
}

void CreateAllHints() {

  CreateGanonText();

  PlacementLog_Msg("\nNOW CREATING HINTS\n");
  const HintSetting& hintSetting = hintSettingTable[Settings::HintDistribution.Value<u8>()];

  u8 remainingDungeonWothHints = hintSetting.dungeonsWothLimit;
  u8 remainingDungeonBarrenHints = hintSetting.dungeonsBarrenLimit;

  // Add 'always' location hints
  if (hintSetting.distTable[static_cast<int>(HintType::Always)].copies > 0) {
    // Only filter locations that had a random item placed at them (e.g. don't get cow locations if shuffle cows is off)
    const auto alwaysHintLocations = FilterFromPool(allLocations, [](const ItemLocation* loc){
        return loc->GetHintCategory() == HintCategory::Always &&
               loc->IsHintable()      && !loc->IsHintedAt();
    });
    for (ItemLocation* location : alwaysHintLocations) {
      CreateLocationHint({location});
    }
  }

  //Add 'trial' location hints
  if (hintSetting.distTable[static_cast<int>(HintType::Trial)].copies > 0) {
    CreateTrialHints();
  }

  //create a vector with each hint type proportional to it's weight in the distribution setting.
  //ootr uses a weighted probability function to decide hint types, but selecting randomly from
  //this vector will do for now
  std::vector<HintType> remainingHintTypes = {};
  for (HintDistributionSetting hds : hintSetting.distTable) {
    remainingHintTypes.insert(remainingHintTypes.end(), hds.weight, hds.type);
  }
  Shuffle(remainingHintTypes);

  //get barren regions
  auto barrenLocations = CalculateBarrenRegions();

  //while there are still gossip stones remaining
  while (FilterFromPool(gossipStoneLocations, [](ItemLocation* loc){return loc->GetPlacedItem() == NoItem;}).size() != 0) {
    //TODO: fixed hint types

    if (remainingHintTypes.empty()) {
      break;
    }

    //get a random hint type from the remaining hints
    HintType type = RandomElement(remainingHintTypes, true);

    PlacementLog_Msg("Attempting to make hint of type: ");
    PlacementLog_Msg(std::to_string(static_cast<int>(type)));
    PlacementLog_Msg("\n");

    //create the appropriate hint for the type
    if (type == HintType::Woth) {
      CreateWothHint(&remainingDungeonWothHints);

    } else if (type == HintType::Barren) {
      CreateBarrenHint(&remainingDungeonBarrenHints, barrenLocations);

    } else if (type == HintType::Sometimes){
      std::vector<ItemLocation*> sometimesHintLocations = FilterFromPool(allLocations, [](ItemLocation* loc){return loc->GetHintCategory() == HintCategory::Sometimes && loc->IsHintable() && !loc->IsHintedAt();});
      CreateLocationHint(sometimesHintLocations);

    } else if (type == HintType::Random) {
      CreateRandomLocationHint();

    } else if (type == HintType::Item) {
      CreateGoodItemHint();

    } else if (type == HintType::Song){
      std::vector<ItemLocation*> songHintLocations = FilterFromPool(allLocations, [](ItemLocation* loc){return loc->IsCategory(Category::cSong) && loc->IsHintable() && !loc->IsHintedAt();});
      CreateLocationHint(songHintLocations);

    } else if (type == HintType::Overworld){
      std::vector<ItemLocation*> overworldHintLocations = FilterFromPool(allLocations, [](ItemLocation* loc){return loc->IsOverworld() && loc->IsHintable() && !loc->IsHintedAt();});
      CreateLocationHint(overworldHintLocations);

    } else if (type == HintType::Dungeon){
      std::vector<ItemLocation*> dungeonHintLocations = FilterFromPool(allLocations, [](ItemLocation* loc){return loc->IsDungeon() && loc->IsHintable() && !loc->IsHintedAt();});
      CreateLocationHint(dungeonHintLocations);

    } else if (type == HintType::Junk) {
      CreateJunkHint();
    }
  }

  //Getting gossip stone locations temporarily sets one location to not be reachable.
  //Call the function one last time to get rid of false positives on locations not
  //being reachable.
  GetAccessibleLocations({});
}
