/**
 * @file word_observations.h
 * @author Chase Geigle
 *
 * All files in META are dual-licensed under the MIT and NCSA licenses. For more
 * details, consult the file LICENSE.mit and LICENSE.ncsa in the root of the
 * project.
 */

#ifndef META_SEQUENCE_HMM_WORD_OBS_H_
#define META_SEQUENCE_HMM_WORD_OBS_H_

#include "meta/meta.h"
#include "meta/sequence/hmm/hmm.h"
#include "meta/stats/multinomial.h"

namespace meta
{
namespace sequence
{
namespace hmm
{

/**
 * A multinomial observation distribution for HMMs.
 */
template <class ObservationType = term_id>
class discrete_observations
{
  public:
    using observation_type = ObservationType;

    discrete_observations(uint64_t num_states,
                          stats::dirichlet<observation_type>&& prior)
        : obs_dist_(num_states, prior)
    {
        // nothing
    }

    discrete_observations blank() const
    {
        return {obs_dist_.size(), prior()};
    }

    const stats::dirichlet<observation_type>& prior() const
    {
        return obs_dist_.front().prior();
    }

    double probability(observation_type obs, state_id s_i) const
    {
        return obs_dist_[s_i].probability(obs);
    }

    void increment(observation_type obs, state_id s_i, double amount)
    {
        obs_dist_[s_i].increment(obs, amount);
    }

  private:
    std::vector<stats::multinomial<observation_type>> obs_dist_;
};
}
}
}
#endif