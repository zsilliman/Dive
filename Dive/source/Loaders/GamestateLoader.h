#pragma once

#include <cugl/cugl.h>
#include "../GameState.h"

using namespace cugl;
using namespace std;

class GamestateLoader : public Loader<GameState> {
	protected:
		/**
		 * Finishes loading the Json file, cleaning up the wait queues.
		 *
		 * Allocating a Json asset can be done safely in a separate thread.
		 * Hence this method is really just an internal helper for convenience.
		 *
		 * This method supports an optional callback function which reports whether
		 * the asset was successfully materialized.
		 *
		 * @param key       The key to access the asset after loading
		 * @param json      The json asset fully loaded
		 * @param callback  An optional callback for asynchronous loading
		 */
		void materialize(const std::string& key, const std::shared_ptr<GameState>& gamestate,
			LoaderCallback callback);

		/**
		 * Internal method to support asset loading.
		 *
		 * This method supports either synchronous or asynchronous loading, as
		 * specified by the given parameter.  If the loading is asynchronous,
		 * the user may specify an optional callback function.
		 *
		 * Json loading is always asynchronously safe, so this method does not
		 * split the loading process.
		 *
		 * @param key       The key to access the asset after loading
		 * @param source    The pathname to the asset
		 * @param callback  An optional callback for asynchronous loading
		 * @param async     Whether the asset was loaded asynchronously
		 *
		 * @return true if the asset was successfully loaded
		 */
		virtual bool read(const std::string& key, const std::string& source,
			LoaderCallback callback, bool async) override;

		/**
		 * Internal method to support asset loading.
		 *
		 * This method supports either synchronous or asynchronous loading, as
		 * specified by the given parameter.  If the loading is asynchronous,
		 * the user may specify an optional callback function.
		 *
		 * Json loading is always asynchronously safe, so this method does not
		 * split the loading process.
		 *
		 * This version of read provides support for JSON directories. A json
		 * directory entry is just a key with a string value for the
		 * path to the asset.
		 *
		 * @param json      The directory entry for the asset
		 * @param callback  An optional callback for asynchronous loading
		 * @param async     Whether the asset was loaded asynchronously
		 *
		 * @return true if the asset was successfully loaded
		 */
		virtual bool read(const std::shared_ptr<JsonValue>& json,
			LoaderCallback callback, bool async) override;

	public:
#pragma mark -
#pragma mark Constructors
		/**
		 * Creates a new, uninitialized Json loader
		 *
		 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a loader on
		 * the heap, use one of the static constructors instead.
		 */
		GamestateLoader() {}

		/**
		 * Disposes all resources and assets of this loader
		 *
		 * Any assets loaded by this object will be immediately released by the
		 * loader.  However, a Json asset may still be available if referenced
		 * by another smart pointer.
		 *
		 * Once the loader is disposed, any attempts to load a new asset will
		 * fail.  You must reinitialize the loader to begin loading assets again.
		 */
		void dispose() override {
			_assets.clear();
			_loader = nullptr;
		}

		/**
		 * Returns a newly allocated music loader.
		 *
		 * This method bootstraps the loader with any initial resources that it
		 * needs to load assets.
		 *
		 * This loader will have no associated threads. That means any asynchronous
		 * loading will fail until a thread is provided via {@link setThreadPool}.
		 *
		 * @return a newly allocated Json loader.
		 */
		static std::shared_ptr<GamestateLoader> alloc() {
			std::shared_ptr<GamestateLoader> result = std::make_shared<GamestateLoader>();
			return (result->init() ? result : nullptr);
		}

		/**
		 * Returns a newly allocated music loader.
		 *
		 * This method bootstraps the loader with any initial resources that it
		 * needs to load assets.
		 *
		 * This loader will have no associated threads. That means any asynchronous
		 * loading will fail until a thread is provided via {@link setThreadPool}.
		 *
		 * @param threads   The thread pool for asynchronous loading
		 *
		 * @return a newly allocated Json loader.
		 */
		static std::shared_ptr<GamestateLoader> alloc(const std::shared_ptr<ThreadPool>& threads) {
			std::shared_ptr<GamestateLoader> result = std::make_shared<GamestateLoader>();
			return (result->init(threads) ? result : nullptr);
		}
	};