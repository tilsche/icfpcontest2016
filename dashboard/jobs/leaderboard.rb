require 'rest-client'
require 'json'

SCHEDULER.every '5s' do

    response = RestClient.get 'http://2016sv.icfpcontest.org/api/snapshot/list', { "X-API-Key" => "191-d1229cf42703fee94dd0df16779b0074"}
    jres = JSON.parse(response)
    snapshots = jres["snapshots"]
    snapshot = snapshots.last
    snapshot_hash = snapshot["snapshot_hash"]

    SCHEDULER.in '2s' do
        response = RestClient.get 'http://2016sv.icfpcontest.org/api/blob/' + snapshot_hash , { "X-API-Key" => "191-d1229cf42703fee94dd0df16779b0074"}

        jres = JSON.parse(response)

        # puts JSON.pretty_generate(jres)

        jleaderboard = jres["leaderboard"]

        jusers = jres["users"]

        leaderboard = []

        i = 1

        has_zih = false

        jleaderboard.each { |user|
            if i <= 19 then
                jusers.each { |u|
                    if u["username"] == user["username"] then
                        leaderboard.push label: "#{i}. " + u["display_name"], value: user["score"]
                    end
                }
            end
            if user["username"] == "191" then
                leaderboard.push label: "#{i}. Zebra Infused Hamsters", value: user["score"]
                has_zih = true
            end

            i = i + 1
        }

        if not has_zih then
            leaderboard.push label: "#{i}. Zebra Infused Hamsters", value: 0.0
        end

        send_event('leaderboard', { items: leaderboard })
    end
end
