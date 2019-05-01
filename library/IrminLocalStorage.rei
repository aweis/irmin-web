/**
 * A store that uses HTML LocalStorage
 **/

/** Configuration values. */
let config: unit => Irmin.config;

/** An in localStorage store for append-only values. */
module Append_only: Irmin.APPEND_ONLY_STORE_MAKER;

/** An in localStorage store with atomic-write guarantees. */
module Atomic_write: Irmin.ATOMIC_WRITE_STORE_MAKER;

/** an in localStorage Irmin store */
module Make: Irmin.S_MAKER;

/** an in localStorage KV store */
module KV: Irmin.KV_MAKER;
