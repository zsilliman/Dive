#include "GamestateLoader.h"

/** What the source name is if we do not know it */
#define UNKNOWN_SOURCE  "<unknown>"

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
void GamestateLoader::materialize(const std::string& key, const std::shared_ptr<GameState>& gamestate,
	LoaderCallback callback) {
	bool success = false;
	if (gamestate != nullptr) {
		_assets[key] = gamestate;
		success = true;
	}

	if (callback != nullptr) {
		callback(key, success);
	}
	_queue.erase(key);
}

/**
 * Internal method to support asset loading.
 *
 * This method supports either synchronous or asynchronous loading, as
 * specified by the given parameter.  If the loading is asynchronous,
 * the user may specify an optional callback function.
 *
 * This method will split the loading across the {@link Music#alloc} and
 * the internal {@link materialize} method.  This ensures that asynchronous
 * loading is safe.
 *
 * @param key       The key to access the asset after loading
 * @param source    The pathname to the asset
 * @param callback  An optional callback for asynchronous loading
 * @param async     Whether the asset was loaded asynchronously
 *
 * @return true if the asset was successfully loaded
 */
bool GamestateLoader::read(const std::string& key, const std::string& source, LoaderCallback callback, bool async) {
	if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
		return false;
	}
	_queue.emplace(key);

	bool success = false;
	if (_loader == nullptr || !async) {
		std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(source);
		std::shared_ptr<JsonValue> json = (reader == nullptr ? nullptr : reader->readJson());
		shared_ptr<GameState> gamestate = GameState::allocFromJson(json);
		success = (gamestate != nullptr);
		materialize(key, gamestate, callback);
	}
	else {
		_loader->addTask([=](void) {
			std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(source);
			std::shared_ptr<JsonValue> json = (reader == nullptr ? nullptr : reader->readJson());
			shared_ptr<GameState> gamestate = GameState::allocFromJson(json);
			Application::get()->schedule([=](void) {
				this->materialize(key, gamestate, callback);
				return false;
			});
		});
	}

	return success;
}

/**
 * Internal method to support asset loading.
 *
 * This method supports either synchronous or asynchronous loading, as
 * specified by the given parameter.  If the loading is asynchronous,
 * the user may specify an optional callback function.
 *
 * This method will split the loading across the {@link Music#alloc} and
 * the internal {@link materialize} method.  This ensures that asynchronous
 * loading is safe.
 *
 * This version of read provides support for JSON directories. A music
 * directory entry has the following values
 *
 *      "file":         The path to the asset
 *      "volume":       This default sound volume (float)
 *
 * @param json      The directory entry for the asset
 * @param callback  An optional callback for asynchronous loading
 * @param async     Whether the asset was loaded asynchronously
 *
 * @return true if the asset was successfully loaded
 */
bool GamestateLoader::read(const std::shared_ptr<JsonValue>& json, LoaderCallback callback, bool async) {
	std::string key = json->key();
	if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
		return false;
	}
	_queue.emplace(key);
	std::string source = json->get("file")->asString(UNKNOWN_SOURCE);

	bool success = false;
	if (_loader == nullptr || !async) {
		std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(source);
		std::shared_ptr<JsonValue> json = (reader == nullptr ? nullptr : reader->readJson());
		shared_ptr<GameState> gamestate = GameState::allocFromJson(json);
		success = (json != nullptr);
		materialize(key, gamestate, callback);
	}
	else {
		_loader->addTask([=](void) {
			std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(source);
			std::shared_ptr<JsonValue> json = (reader == nullptr ? nullptr : reader->readJson());
			shared_ptr<GameState> gamestate = GameState::allocFromJson(json);
			Application::get()->schedule([=](void) {
				this->materialize(key, gamestate, callback);
				return false;
			});
		});
	}

	return success;
}

