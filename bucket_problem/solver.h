
#pragma once

#include <iostream>
#include <vector>

#include "alias\ad.h"

namespace ad {

template<typename Slot, typename t_Identifier>
class Bucket {
  
  const t_Identifier identifier_;
  
  const U64 balls_num_;
  
  const std::vector<Slot> slots_;
  
 public:
  
  const t_Identifier &Identifier() const {return identifier_;}
  const U64 &BallsNum() const {return balls_num_;}
  const std::vector<Slot> &Slots() const {return slots_;}
  
  Bucket(t_Identifier identifier, U64 balls_num, std::vector<Slot> slots):
  identifier_(identifier),
  balls_num_(balls_num),
  slots_(slots)
  {}
  
};

// This is a solver for bucket problem.
// In case that data provided via Process() function is nonsensical,
// behaviour of this class is undefined.
template<typename Slot, typename t_Identifier>
class BucketProblemSolver {
  
  using Bucket = Bucket<Slot, t_Identifier>;
  
  // Concluded status of slots not belonging to any bucket.
  // 1 means that each of them contains a ball.
  // 0 means that they contain no balls.
  // -1 means there is no conclusion and there was no attempt to create it.
  // -2 means there is no conclusion and there was an attempt to create it.
  // "Attempt" means call of Process() function with additional info:
  // global_balls_num and separate_slots.
  // TODO: consider using enum.
  U64 separate_slots_concluded_status_;
  
  std::vector<Bucket> considered_buckets_;
  
  // TODO: consider using dictionary.
  std::vector<Slot> considered_slots_;
  
  std::vector<Slot> separate_slots_;
  
  // Rule (element of considered_rules_) is meant to be
  // a std::vector<U64> v such that v[0] is number of trues on indices
  // v[1], ..., v.back() of the elements of possible_arrangements_.
  std::vector<std::vector<U64>> considered_rules_;
  
  // rule_applicability_[i] tells if rule at i-th index of considered_rules_ is applicable,
  // that means if it has all corresponding slots indexed in current considered_arrangements_.
  // Once one of that values turns true, it will never be false again.
  std::vector<bool> rule_applicability_;
  
  std::vector<std::vector<bool>> possible_arrangements_;
  
  // Adds slot at the end of considered_slots_.
  // Does not check if added slot is duplicate;
  // if it is behaviour of whole alorithm will likely break.
  void ConsiderSlot(const Slot &slot) {
    considered_slots_.push_back(slot);
  }
  
  // Returns index of slot in considered_slots_, adds it if it is not in this vector yet.
  U64 Index(const Slot &slot) {
    U64 j;
    for (j = 0; j != considered_slots_.size(); j++) {
      if (slot == considered_slots_[j]) {
        break;
      }
    }
    // Loop above guarantees added slot to be not a duplicate.
    if (j == considered_slots_.size()) {
      ConsiderSlot(slot);
    }
    return j;
  }
  
  // Creates rule according to given bucket and adds it at the end of considered_rules_ vector.
  // As a side effect adds bucket's slots to considered_slots_ if necessary.
  // Does not check if rule to be created is duplicate*;
  // even if it is behaviour of whole algorithm will not break, it will be just bit slower.
  // *duplicate rule may, or may not, be created from bucket that already was used to create rule.
  void ConsiderRuleAccordingTo(const Bucket &bucket) {
    std::vector<U64> rule = {bucket.BallsNum()};
    for (U64 i = bucket.Slots().size() - 1; i != U64(-1); i--) {
      const Slot &slot = bucket.Slots()[i];
      rule.push_back(Index(slot));
    }
    considered_rules_.push_back(rule);
    rule_applicability_.push_back(0);
  }
  
  // Creates and adds corresponding rule to considered_rules_.
  // Adds bucket at the end of considered_buckets_.
  // As a side effect adds bucket's slots to considered_slots_ if necessary.
  // If given bucket was already considered (one with the same identifier),
  // call of this function has no effect.
  void ConsiderBucket(const Bucket &bucket) {
    for (U64 i = considered_buckets_.size() - 1; i != U64(-1); i--) {
      if (bucket.Identifier() == considered_buckets_[i].Identifier()) {return;}
    }
    // Loop above guarantees that the same bucket will not be considered multiple times.
    ConsiderRuleAccordingTo(bucket);
    considered_buckets_.push_back(bucket);
  }
  
  // Returns true iff rule of given index is applicable.
  bool RuleIsApplicable(const U64 &rule_index) {
    return rule_applicability_[rule_index];
  }
  
  // Verifies if rule is applicable,
  // in other words updates rule_applicability_[rule_index] value.
  void VerifyRuleApplicability(const U64 &rule_index) {
    if (rule_applicability_[rule_index]) {return;}
    const U64 limit = possible_arrangements_[0].size();
    // Why iterating through the index 0 below makes sense is left as an exercise.
    for (U64 i : considered_rules_[rule_index]) {
      if (i >= limit) {return;}
    }
    rule_applicability_[rule_index] = 1;
  }
  
  // Returns true iff arrangement satisfies rule.
  bool Satisfies(const std::vector<bool> &arrangement, const std::vector<U64> &rule) const {
    U64 sum = -rule[0];
    for (U64 i = rule.size() - 1; i > U64(0); i--) {
      sum += arrangement[rule[i]];
    }
    return !sum;
  }
  
  // Quickly removes index-th arrangement from possible_arrangements_.
  void RemovePossibleArrangement(const U64 &index) {
    std::swap(possible_arrangements_[index], possible_arrangements_.back());
    possible_arrangements_.pop_back();
  }
  
  // Removes elements of possible_arrangements_ that do not satisfy all considerd_rules_.
  // As a side effect verifies rules' applicability as it is necessary to avoid UB.
  void ClearPossibleArrangements() {
    for (U64 j = considered_rules_.size() - 1; j != U64(-1); j--) {
      VerifyRuleApplicability(j);
    }
    // It is important that loop iterates backward here because of how
    // RemovePossibleArrangement() function works.
    for (U64 i = possible_arrangements_.size() - 1; i != U64(-1); i--) {
      for (U64 j = considered_rules_.size() - 1; j != U64(-1); j--) {
        if (!RuleIsApplicable(j)) {continue;}
        if (!Satisfies(possible_arrangements_[i], considered_rules_[j])) {
          RemovePossibleArrangement(i);
          break;
        }
      }
    }
  }
  
  // Modifies possible_arrangements_ in a way, such that it contains old arrangements
  // with all possible bool ranges of length new_indices_num appended.
  // This multiplies size of possible_arrangements_ by 2^new_indices_num.
  void ConsiderNewArrangements(U64 new_indices_num = 1) {
    for (U64 j = 0; j != new_indices_num; j++) {
      for (U64 i = possible_arrangements_.size() - 1; i != U64(-1); i--) {
        possible_arrangements_.push_back(possible_arrangements_[i]);
      }
      for (U64 i = possible_arrangements_.size() / 2 - 1; i != U64(-1); i--) {
        possible_arrangements_[i].push_back(0);
        possible_arrangements_[i + possible_arrangements_.size() / 2].push_back(1);
      }
    }
  }
  
  void PrintPossibleArrangements() {
    std::cout << std::endl;
    for (const std::vector<bool> &arrangement : possible_arrangements_) {
      for (const bool &val : arrangement) {
        std::cout << val << " ";
      }
      std::cout << std::endl;
    }    
  }
  
  void PrintConsideredRules() {
    for (const std::vector<U64> &rule : considered_rules_) {
      for (const U64 &val : rule) {
        std::cout << val << " ";
      }
      std::cout << std::endl;
    }    
  }
  
  // Returns std::vector<U64> of indices of arrangement that can be concluded to be val,
  // in other words returns std::vector<U64> containing every i such that a[i] == val
  // for a being any element of possible_arrangements_.
  std::vector<U64> ArrangementIndicesConcludedAs(const bool &val) const {
    std::vector<U64> concluded_indices;
    for (U64 i = 0; i < considered_slots_.size(); i++) {
      bool concluded = 1;
      for (U64 j = 0; j < possible_arrangements_.size(); j++) {
        if (possible_arrangements_[j][i] != val) {
          concluded = 0;
          break;
        };
      }
      if (concluded) {
        concluded_indices.push_back(i);
      }
    }
    return concluded_indices;
  }
  
 public:
  
  // Processes a bucket, or does nothing if additional info (global_balls_num and separate_slots)
  // was already processed because it would make no sense and would lead to illogical behaviour.
  void Process(const Bucket &bucket) {
    if (separate_slots_concluded_status_ != U64(-1)) {return;}
    U64 current_considered_slots_num = considered_slots_.size();
    ConsiderBucket(bucket);
    U64 new_considered_slots_num = considered_slots_.size() - current_considered_slots_num;
    ConsiderNewArrangements(new_considered_slots_num);
    ClearPossibleArrangements();
  }
  
  // Processes additional info (global_balls_num and separate_slots) or does nothing if it
  // was already processed because it would make no sense and would lead to illogical behaviour.
  // Generally processing additional info makes sense only after all buckets are processed.
  // It would make sense to pass number of separate slots here instead of vector of them,
  // although it would make complications on the other end.
  void Process(
    // Number of all balls, ones in buckets and ones in separate slots together.
    const U64 &global_balls_num,
    // Slots not belonging to any bucket.
    const std::vector<Slot> &separate_slots
  ) {
    if (separate_slots_concluded_status_ != U64(-1)) {return;}
    separate_slots_ = separate_slots;
    U64 arrangement_balls_num_min = -1;
    U64 arrangement_balls_num_max = 0;
    for (U64 i = possible_arrangements_.size() - 1; i != U64(-1); i--) {
      U64 arrangement_balls_num = 0;
      for (const bool &b : possible_arrangements_[i]) {
        arrangement_balls_num += b;
      }
      // Remove arrangement if it has impossibly many balls.
      if (arrangement_balls_num > global_balls_num) {
        RemovePossibleArrangement(i);
      } else
      // Remove arrangement if it has impossibly few balls.
      if (arrangement_balls_num + separate_slots_.size() < global_balls_num) {
        RemovePossibleArrangement(i);
      } else 
      // min & max arrangement_balls_num only if it corresponds to kept arrangement.
      {
        arrangement_balls_num_min = std::min(arrangement_balls_num_min, arrangement_balls_num);
        arrangement_balls_num_max = std::max(arrangement_balls_num_max, arrangement_balls_num);
      }
    }
    if (
      arrangement_balls_num_min == arrangement_balls_num_max
    ) {
      if (arrangement_balls_num_min == global_balls_num) {
        separate_slots_concluded_status_ = 0;
      } else if (arrangement_balls_num_min + separate_slots_.size() == global_balls_num) {
        separate_slots_concluded_status_ = 1;
      } else {
        separate_slots_concluded_status_ = -2;
      }
    } else {
      separate_slots_concluded_status_ = -2;
    }
  }
  
  // Like ArrangementIndicesConcludedAs() but returns corresponding slots instead of indices,
  // moreover returned vector contains separate_slots_ if it is concluded if they are
  // full of bombs (val == true) or empty (val == false).
  std::vector<Slot> ArrangementSlotsConcludedAs(const bool &val) const {
    std::vector<Slot> concluded_slots;
    if (separate_slots_concluded_status_ == U64(val)) {
      concluded_slots = separate_slots_;
    }
    std::vector<U64> concluded_indices = ArrangementIndicesConcludedAs(val);
    for (const U64 &i : concluded_indices) {
      concluded_slots.push_back(considered_slots_[i]);
    }
    return concluded_slots;
  }
  
  BucketProblemSolver():
  separate_slots_concluded_status_(-1),
  considered_buckets_({}),
  considered_slots_({}),
  considered_rules_({}),
  possible_arrangements_({{}})
  {}
  
};

}
