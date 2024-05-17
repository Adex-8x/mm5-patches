#include <pmdsky.h>
#include <cot.h>

// Called when using items. Should return true if a custom effect was applied.
bool CustomApplyItemEffect(
  struct entity* user, struct entity* target, struct item* item, bool is_thrown
) {
  switch (item->id.val) {
    default:
      return false;
  }
}
