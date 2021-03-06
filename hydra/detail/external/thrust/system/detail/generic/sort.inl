/*
 *  Copyright 2008-2013 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#pragma once

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/system/detail/generic/sort.h>
#include <hydra/detail/external/thrust/functional.h>
#include <hydra/detail/external/thrust/iterator/iterator_traits.h>
#include <hydra/detail/external/thrust/distance.h>
#include <hydra/detail/external/thrust/functional.h>
#include <hydra/detail/external/thrust/find.h>
#include <hydra/detail/external/thrust/iterator/zip_iterator.h>
#include <hydra/detail/external/thrust/tuple.h>
#include <hydra/detail/external/thrust/detail/internal_functional.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{


template<typename DerivedPolicy,
         typename RandomAccessIterator>
__hydra_host__ __hydra_device__
  void sort(thrust::execution_policy<DerivedPolicy> &exec,
            RandomAccessIterator first,
            RandomAccessIterator last)
{
  typedef typename thrust::iterator_value<RandomAccessIterator>::type value_type; 
  thrust::sort(exec, first, last, thrust::less<value_type>());
} // end sort()


template<typename DerivedPolicy,
         typename RandomAccessIterator,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
  void sort(thrust::execution_policy<DerivedPolicy> &exec,
            RandomAccessIterator first,
            RandomAccessIterator last,
            StrictWeakOrdering comp)
{
  // implement with stable_sort
  thrust::stable_sort(exec, first, last, comp);
} // end sort()


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2>
__hydra_host__ __hydra_device__
  void sort_by_key(thrust::execution_policy<DerivedPolicy> &exec,
                   RandomAccessIterator1 keys_first,
                   RandomAccessIterator1 keys_last,
                   RandomAccessIterator2 values_first)
{
  typedef typename thrust::iterator_value<RandomAccessIterator1>::type value_type;
  thrust::sort_by_key(exec, keys_first, keys_last, values_first, thrust::less<value_type>());
} // end sort_by_key()


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
  void sort_by_key(thrust::execution_policy<DerivedPolicy> &exec,
                   RandomAccessIterator1 keys_first,
                   RandomAccessIterator1 keys_last,
                   RandomAccessIterator2 values_first,
                   StrictWeakOrdering comp)
{
  // implement with stable_sort_by_key
  thrust::stable_sort_by_key(exec, keys_first, keys_last, values_first, comp);
} // end sort_by_key()


template<typename DerivedPolicy,
         typename RandomAccessIterator>
__hydra_host__ __hydra_device__
  void stable_sort(thrust::execution_policy<DerivedPolicy> &exec,
                   RandomAccessIterator first,
                   RandomAccessIterator last)
{
  typedef typename thrust::iterator_value<RandomAccessIterator>::type value_type;
  thrust::stable_sort(exec, first, last, thrust::less<value_type>());
} // end stable_sort()


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2>
__hydra_host__ __hydra_device__
  void stable_sort_by_key(thrust::execution_policy<DerivedPolicy> &exec,
                          RandomAccessIterator1 keys_first,
                          RandomAccessIterator1 keys_last,
                          RandomAccessIterator2 values_first)
{
  typedef typename iterator_value<RandomAccessIterator1>::type value_type;
  thrust::stable_sort_by_key(exec, keys_first, keys_last, values_first, thrust::less<value_type>());
} // end stable_sort_by_key()


template<typename DerivedPolicy, typename ForwardIterator>
__hydra_host__ __hydra_device__
  bool is_sorted(thrust::execution_policy<DerivedPolicy> &exec,
                 ForwardIterator first,
                 ForwardIterator last)
{
  return thrust::is_sorted_until(exec, first, last) == last;
} // end is_sorted()


template<typename DerivedPolicy,
         typename ForwardIterator,
         typename Compare>
__hydra_host__ __hydra_device__
  bool is_sorted(thrust::execution_policy<DerivedPolicy> &exec,
                 ForwardIterator first,
                 ForwardIterator last,
                 Compare comp)
{
  return thrust::is_sorted_until(exec, first, last, comp) == last;
} // end is_sorted()


template<typename DerivedPolicy, typename ForwardIterator>
__hydra_host__ __hydra_device__
  ForwardIterator is_sorted_until(thrust::execution_policy<DerivedPolicy> &exec,
                                  ForwardIterator first,
                                  ForwardIterator last)
{
  typedef typename thrust::iterator_value<ForwardIterator>::type InputType;

  return thrust::is_sorted_until(exec, first, last, thrust::less<InputType>());
} // end is_sorted_until()


template<typename DerivedPolicy,
         typename ForwardIterator,
         typename Compare>
__hydra_host__ __hydra_device__
  ForwardIterator is_sorted_until(thrust::execution_policy<DerivedPolicy> &exec,
                                  ForwardIterator first,
                                  ForwardIterator last,
                                  Compare comp)
{
  if(thrust::distance(first,last) < 2) return last;

  typedef thrust::tuple<ForwardIterator,ForwardIterator> IteratorTuple;
  typedef thrust::zip_iterator<IteratorTuple>            ZipIterator;

  ForwardIterator first_plus_one = first;
  thrust::advance(first_plus_one, 1);

  ZipIterator zipped_first = thrust::make_zip_iterator(thrust::make_tuple(first_plus_one, first));
  ZipIterator zipped_last  = thrust::make_zip_iterator(thrust::make_tuple(last, first));

  return thrust::get<0>(thrust::find_if(exec, zipped_first, zipped_last, thrust::detail::tuple_binary_predicate<Compare>(comp)).get_iterator_tuple());
} // end is_sorted_until()


template<typename DerivedPolicy,
         typename RandomAccessIterator,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
  void stable_sort(thrust::execution_policy<DerivedPolicy> & /*exec*/,
                   RandomAccessIterator /*first*/,
                   RandomAccessIterator /*last*/,
                   StrictWeakOrdering /*comp*/)
{
  // unimplemented primitive
  HYDRA_THRUST_STATIC_ASSERT( (thrust::detail::depend_on_instantiation<RandomAccessIterator, false>::value) );
} // end stable_sort()


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
  void stable_sort_by_key(thrust::execution_policy<DerivedPolicy> &/*exec*/,
                          RandomAccessIterator1 /*keys_first*/,
                          RandomAccessIterator1 /*keys_last*/,
                          RandomAccessIterator2 /*values_first*/,
                          StrictWeakOrdering /*comp*/)
{
  // unimplemented primitive
  HYDRA_THRUST_STATIC_ASSERT( (thrust::detail::depend_on_instantiation<RandomAccessIterator1, false>::value) );
} // end stable_sort_by_key()


} // end generic
} // end detail
} // end system
} // end thrust

HYDRA_EXTERNAL_NAMESPACE_END
